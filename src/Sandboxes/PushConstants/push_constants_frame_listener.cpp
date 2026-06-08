#include "push_constants_frame_listener.h"

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
        , _vertexBuffer(nullptr)
        , _pushConstantsArray()
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    PushConstantsFrameListener::~PushConstantsFrameListener()
    {
        _Finalize();
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
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            { -0.1f,  0.1f },
            {  0.1f,  0.1f },
            { -0.1f, -0.1f }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize }));
        _vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        renderer.CopyBuffers(stagingBuffer, {
            { *_vertexBuffer.get(), 0, 0, vertexBufferSize }
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
        textureAttachmentManager.AddTextureAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment, VK_ImageAspect_Color);
        textureAttachmentManager.AddTextureAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment, VK_ImageAspect_DepthStencil);

        vulkanDevice.GetPipelineManager().AddPipelineLayout(PipelineLayout_SID, {}, {
            { VK_ShaderStage_Vertex, 0, sizeof(PushConstantsData) }
        });

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("push_constants.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("push_constants.frag.spv");
        const auto vertexShaderModule = vulkanDevice.CreateShaderModule(vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.CreateShaderModule(fragmentShaderPath);
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBuffer, VertexBufferBinding);
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams);
    }
    //--------------------------------------------------------------------------

    void PushConstantsFrameListener::_Finalize()
    {
        _vertexBuffer.reset();
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
        renderer.BindVertexBuffers(VertexBufferBinding, { _vertexBuffer->GetVkBuffer() }, { 0 });

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