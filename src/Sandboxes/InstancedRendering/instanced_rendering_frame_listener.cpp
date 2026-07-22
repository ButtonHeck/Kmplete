#include "instanced_rendering_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_attachment_manager.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "Pipeline"_sid;

    static constexpr auto VertexBufferBinding = 0;
    static constexpr auto InstancePositionBufferBinding = 1;
    static constexpr auto InstanceColorBufferBinding = 2;

    static constexpr auto VertexPositionAttributeIndex = 0;
    static constexpr auto VertexPositionInstancedAttributeIndex = 1;
    static constexpr auto VertexColorInstancedAttributeIndex = 2;

    static constexpr auto NumInstancesInRow = 9;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexShaderModule_SID = "vertex_shader"_sid;
    static constexpr auto FragmentShaderModule_SID = "fragment_shader"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffer"_sid;
    static constexpr auto VertexBufferPosInstanced_SID = "vertex_buffer_pos_instanced"_sid;
    static constexpr auto VertexBufferColorsInstanced_SID = "vertex_buffer_colors_instanced"_sid;
    static constexpr auto IndexBuffer_SID = "index_buffer"_sid;


    namespace
    {
        struct Vertex
        {
            float position[2];
        };

        struct Color
        {
            float colors[4];
        };
    }

    using namespace Graphics::VKBits;


    InstancedRenderingFrameListener::InstancedRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _indexCount(0)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            { -0.1f,   0.1f },
            {  0.1f,   0.1f },
            { -0.09f, -0.1f }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<Vertex> verticesInstanced{
            // 9 instanced drawn without indices (top row)
            { -0.8f, -0.8f },
            { -0.6f, -0.8f },
            { -0.4f, -0.8f },
            { -0.2f, -0.8f },
            {  0.0f, -0.8f },
            {  0.2f, -0.8f },
            {  0.4f, -0.8f },
            {  0.6f, -0.8f },
            {  0.8f, -0.8f },

            // 9 instanced drawn with indices (middle row)
            { -0.8f, -0.2f },
            { -0.6f, -0.2f },
            { -0.4f, -0.2f },
            { -0.2f, -0.2f },
            {  0.0f, -0.2f },
            {  0.2f, -0.2f },
            {  0.4f, -0.2f },
            {  0.6f, -0.2f },
            {  0.8f, -0.2f },
        };
        const auto vertexInstancedBufferSize = UInt32(verticesInstanced.size() * sizeof(Vertex));

        const Vector<Color> verticesColorsInstanced{
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 0.5f, 0.5f, 0.5f, 1.0f }
        };
        const auto vertexColorsInstancedBufferSize = UInt32(verticesColorsInstanced.size() * sizeof(Color));

        const Vector<UInt32> indices{ 0, 1, 2 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize + indexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)verticesInstanced.data(), vertexInstancedBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + vertexInstancedBufferSize, (char*)verticesColorsInstanced.data(), vertexColorsInstancedBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
        auto vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        vulkanBufferManager.CreateVertexBuffer(VertexBufferPosInstanced_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexInstancedBufferSize });
        auto vertexBufferPosInstanced = vulkanBufferManager.GetVertexBuffer(VertexBufferPosInstanced_SID);
        vertexBufferPosInstanced->AddLayout(Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionInstancedAttributeIndex }
        }, "instanced"_true));

        vulkanBufferManager.CreateVertexBuffer(VertexBufferColorsInstanced_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexColorsInstancedBufferSize });
        auto vertexBufferColorsInstanced = vulkanBufferManager.GetVertexBuffer(VertexBufferColorsInstanced_SID);
        vertexBufferColorsInstanced->AddLayout(Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, VertexColorInstancedAttributeIndex }
        }, "instanced"_true));

        vulkanBufferManager.CreateIndexBuffer(IndexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, indexBufferSize });
        auto indexBuffer = vulkanBufferManager.GetBuffer(IndexBuffer_SID);

        renderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize },
            { *vertexBufferPosInstanced, vertexBufferSize, 0, vertexInstancedBufferSize },
            { *vertexBufferColorsInstanced, vertexBufferSize + vertexInstancedBufferSize, 0, vertexColorsInstancedBufferSize },
            { *indexBuffer, vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize, 0, indexBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayout(PipelineLayout_SID, {});

        auto& shaderManager = vulkanDevice.GetShaderManager();
        shaderManager.AddShaderModules({
            { VertexShaderModule_SID, Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/instanced_rendering.vert.spv"), Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Vertex },
            { FragmentShaderModule_SID, Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/instanced_rendering.frag.spv"), Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Fragment }
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
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBufferPosInstanced_SID), InstancePositionBufferBinding);
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBufferColorsInstanced_SID), InstanceColorBufferBinding);
        pipelineParams.AddVertexInputBindingsDivisors({ { InstanceColorBufferBinding, 2 } }); // only color divisor set to 2, position divisor default 1 is ok
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "instanced_rendering_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetFramebufferSize().x), .height = float(_mainWindow.GetFramebufferSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, {
            vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer(),
            vulkanBufferManager.GetVertexBuffer(VertexBufferPosInstanced_SID)->GetVkBuffer(),
            vulkanBufferManager.GetVertexBuffer(VertexBufferColorsInstanced_SID)->GetVkBuffer() 
        }, { 0, 0, 0 });
        renderer.BindIndexBuffer(*vulkanBufferManager.GetBuffer(IndexBuffer_SID));

        auto imageBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_DepthStencil_PrepareWriting;
        renderer.InsertImageMemoryBarrier(vulkanTextureAttachmentManager.GetTextureAttachment(MS_DepthStencilAttachment), imageBarrierParameters);

        auto colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
            MS_ColorAttachment, 0ULL, VK_Resolve_Average, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true
        );
        auto depthStencilAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_DepthStencil_ClearStore,
            MS_DepthStencilAttachment, 0ULL, VK_Resolve_None, VK_ImageLayout_DontCare
        );

        renderer.BeginRendering(drawArea, { colorAttachmentInfo }, depthStencilAttachmentInfo);
        renderer.Draw(3, NumInstancesInRow, 0, 0);
        renderer.EndRendering();

        colorAttachmentInfo.loadOp = VK_AttachmentLoad_Load;
        depthStencilAttachmentInfo.loadOp = VK_AttachmentLoad_Load;

        renderer.BeginRendering(drawArea, { colorAttachmentInfo }, depthStencilAttachmentInfo);
        renderer.BindVertexBuffers(InstancePositionBufferBinding, { vulkanBufferManager.GetVertexBuffer(VertexBufferPosInstanced_SID)->GetVkBuffer() }, { sizeof(Vertex) * NumInstancesInRow });
        renderer.DrawIndexed(3, NumInstancesInRow, 0, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}