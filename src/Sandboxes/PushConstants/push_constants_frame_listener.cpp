#include "push_constants_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "Pipeline"_sid;

    static constexpr auto VertexBufferBinding = 0;

    static constexpr auto VertexPositionAttributeIndex = 0;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexShaderModule_SID = "vertex_shader"_sid;
    static constexpr auto FragmentShaderModule_SID = "fragment_shader"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffer"_sid;


    namespace
    {
        struct Vertex
        {
            float position[2];
        };
    }

    using namespace Graphics::VKBits;


    PushConstantsFrameListener::PushConstantsFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _pushConstantsArray()
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void PushConstantsFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
    }
    //--------------------------------------------------------------------------

    void PushConstantsFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            { -0.1f,  0.1f },
            {  0.1f,  0.1f },
            { -0.1f, -0.1f }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
        auto vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        renderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize }
        }, vulkanDevice.GetGraphicsQueue());

        for (auto i = 0; i < InstancesCount; i++)
        {
            _pushConstantsArray[i].position = Math::Vec4F(-0.6f + i * 0.2f, 0.0f, 0.0f, 0.0f);
            const auto colorComponent = 0.1f + i * 0.1f;
            _pushConstantsArray[i].color = Math::Vec4F(colorComponent, colorComponent, colorComponent, 1.0f);
        }
    }
    //--------------------------------------------------------------------------

    void PushConstantsFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayout(PipelineLayout_SID, {}, { { VK_ShaderStage_Vertex, 0, sizeof(PushConstantsData) } });

        auto& shaderManager = vulkanDevice.GetShaderManager();
        shaderManager.AddShaderModules({
            { VertexShaderModule_SID, String(KMP_SANDBOX_RESOURCES_FOLDER).append("push_constants.vert.spv") },
            { FragmentShaderModule_SID, String(KMP_SANDBOX_RESOURCES_FOLDER).append("push_constants.frag.spv") }
        });
        const auto shaderStages = shaderManager.GetShaderStageCreateInfos({
            { VertexShaderModule_SID, VK_ShaderStage_Vertex, "main" },
            { FragmentShaderModule_SID, VK_ShaderStage_Fragment, "main" }
        });

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID), VertexBufferBinding);
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "push_constants_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void PushConstantsFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer() }, { 0 });

        auto imageBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_DepthStencil_PrepareWriting;
        renderer.InsertImageMemoryBarrier(vulkanTextureAttachmentManager.GetTextureAttachment(MS_DepthStencilAttachment), imageBarrierParameters);

        const auto colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
            MS_ColorAttachment, 0ULL, VK_Resolve_Average, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true
        );
        const auto depthStencilAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_DepthStencil_ClearStore,
            MS_DepthStencilAttachment, 0ULL, VK_Resolve_None, VK_ImageLayout_DontCare
        );

        renderer.BeginRendering(drawArea, { colorAttachmentInfo }, depthStencilAttachmentInfo);
        for (auto i = 0; i < InstancesCount; i++)
        {
            renderer.PushConstants(PipelineLayout_SID, VK_ShaderStage_Vertex, 0, sizeof(PushConstantsData), &_pushConstantsArray[i]);
            renderer.Draw(3, 1, 0, 0);
        }
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}