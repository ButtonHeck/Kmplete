#include "multiple_pipelines_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_FixedColor_Fill_SID =    "Pipeline_FixedColor_Fill"_sid;
    static constexpr auto Pipeline_FixedColor_Line_SID =    "Pipeline_FixedColor_Line"_sid;
    static constexpr auto Pipeline_BufferedColor_Fill_SID = "Pipeline_BufferedColor_Fill"_sid;
    static constexpr auto Pipeline_BufferedColor_Line_SID = "Pipeline_BufferedColor_Line"_sid;

    static constexpr auto VertexShader_FixedColor_SID =         "FixedColor_vertex"_sid;
    static constexpr auto FragmentShader_FixedColor_SID =       "FixedColor_fragment"_sid;
    static constexpr auto VertexShader_BufferedColor_SID =      "BufferedColor_vertex"_sid;
    static constexpr auto FragmentShader_BufferedColor_SID =    "BufferedColor_fragment"_sid;

    static constexpr auto VertexBufferBinding = 0;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexBufferFixed_SID = "vertex_buffer_fixed"_sid;
    static constexpr auto VertexBufferBuffered_SID = "vertex_buffer_buffered"_sid;


    namespace
    {
        struct FixedColorVertex
        {
            float position[2];
        };

        struct BufferedColorVertex
        {
            float position[2];
            float color[4];
        };
    }

    using namespace Graphics::VKBits;


    MultiplePipelinesFrameListener::MultiplePipelinesFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeBuffers(vulkanDevice);
        _InitializePipelines(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<FixedColorVertex> fixedColorVertices{
            // Top-left
            { -0.95f, -0.05f },
            { -0.05f, -0.05f },
            { -0.95f, -0.95f },

            // Bottom-left
            { -0.95f,  0.95f },
            { -0.05f,  0.95f },
            { -0.95f,  0.05f }
        };
        const auto fixedColorBufferSize = UInt32(fixedColorVertices.size() * sizeof(FixedColorVertex));

        const Vector<BufferedColorVertex> bufferedColorVertices{
            // Top-right
            { { 0.05f, -0.05f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.95f, -0.05f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.05f, -0.95f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

            // Bottom-right
            { { 0.05f,  0.95f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.95f,  0.95f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.05f,  0.05f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        const auto bufferedColorBufferSize = UInt32(bufferedColorVertices.size() * sizeof(BufferedColorVertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, fixedColorBufferSize + bufferedColorBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)fixedColorVertices.data(), fixedColorBufferSize);
        stagingBuffer.CopyToMappedMemory(fixedColorBufferSize, (char*)bufferedColorVertices.data(), bufferedColorBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBufferFixed_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, fixedColorBufferSize });
        auto vertexBufferFixedColor = vulkanBufferManager.GetVertexBuffer(VertexBufferFixed_SID);
        vertexBufferFixedColor->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 0 }
        });

        vulkanBufferManager.CreateVertexBuffer(VertexBufferBuffered_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, bufferedColorBufferSize });
        auto vertexBufferBufferedColor = vulkanBufferManager.GetVertexBuffer(VertexBufferBuffered_SID);
        vertexBufferBufferedColor->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 0 },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, 1 },
        });

        renderer.CopyBuffers(stagingBuffer, {
            { *vertexBufferFixedColor, 0, 0, fixedColorBufferSize },
            { *vertexBufferBufferedColor, fixedColorBufferSize, 0, bufferedColorBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::_InitializePipelines(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();

        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayout(PipelineLayout_SID, {});

        auto& shaderManager = vulkanDevice.GetShaderManager();
        shaderManager.AddShaderModules({
            { VertexShader_FixedColor_SID, Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/multiple_pipelines_fixed_color.vert.spv"), Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Vertex },
            { FragmentShader_FixedColor_SID, Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/multiple_pipelines_fixed_color.frag.spv"), Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Fragment },
            { VertexShader_BufferedColor_SID, Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/multiple_pipelines_buffered_color.vert.spv"),  Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Vertex },
            { FragmentShader_BufferedColor_SID, Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/multiple_pipelines_buffered_color.frag.spv"), Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Fragment }
        });
        const auto fixedColorShaderStages = shaderManager.GetShaderStageCreateInfos({
            { VertexShader_FixedColor_SID, VK_ShaderStage_Vertex, "main" },
            { FragmentShader_FixedColor_SID, VK_ShaderStage_Fragment, "main" }
        });
        const auto bufferedColorShaderStages = shaderManager.GetShaderStageCreateInfos({
            { VertexShader_BufferedColor_SID, VK_ShaderStage_Vertex, "main" },
            { FragmentShader_BufferedColor_SID, VK_ShaderStage_Fragment, "main" }
        });

        auto pipelineFixedColorFillParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineFixedColorFillParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineFixedColorFillParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineFixedColorFillParams.AddShaderStages(fixedColorShaderStages);
        pipelineFixedColorFillParams.AddVertexBufferAttributesBindings(*vulkanBufferManager.GetVertexBuffer(VertexBufferFixed_SID), VertexBufferBinding);
        pipelineFixedColorFillParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        auto pipelineFixedColorLineParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineFixedColorFillParams, 
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::All
        );
        pipelineFixedColorLineParams.SetPolygonMode(VK_Polygon_Line);

        auto pipelineBufferedColorFillParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineFixedColorFillParams, 
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::ColorAttachmentInfos |
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::DynamicStates
        );
        pipelineBufferedColorFillParams.AddVertexBufferAttributesBindings(*vulkanBufferManager.GetVertexBuffer(VertexBufferBuffered_SID), VertexBufferBinding);
        pipelineBufferedColorFillParams.AddShaderStages(bufferedColorShaderStages);

        auto pipelineBufferedColorLineParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineBufferedColorFillParams,
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::All
        );
        pipelineBufferedColorLineParams.SetPolygonMode(VK_Polygon_Line);
        pipelineBufferedColorLineParams.SetLineRasterizationMode(VK_LineRasterization_RectangularSmooth);
        pipelineBufferedColorLineParams.SetLineWidth(8.0f);
        pipelineBufferedColorLineParams.SetLineStipple(true, 1, 1);

        const auto& applicationDataPath = ApplicationContext::GetApplicationDataPath();
        pipelineManager.AddGraphicsPipeline(Pipeline_FixedColor_Fill_SID, PipelineLayout_SID, pipelineFixedColorFillParams, applicationDataPath / "multiple_pipelines_fixed_color_fill_pipeline_cache.bin");
        pipelineManager.AddGraphicsPipeline(Pipeline_FixedColor_Line_SID, PipelineLayout_SID, pipelineFixedColorLineParams, applicationDataPath / "multiple_pipelines_fixed_color_line_pipeline_cache.bin");
        pipelineManager.AddGraphicsPipeline(Pipeline_BufferedColor_Fill_SID, PipelineLayout_SID, pipelineBufferedColorFillParams, applicationDataPath / "multiple_pipelines_buffered_color_fill_pipeline_cache.bin");
        pipelineManager.AddGraphicsPipeline(Pipeline_BufferedColor_Line_SID, PipelineLayout_SID, pipelineBufferedColorLineParams, applicationDataPath / "multiple_pipelines_buffered_color_line_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

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

        // common setup for all pipelines
        renderer.BeginRendering(drawArea, { colorAttachmentInfo }, depthStencilAttachmentInfo);
        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());

        // fixed color drawing
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanBufferManager.GetVertexBuffer(VertexBufferFixed_SID)->GetVkBuffer()}, {0});
        renderer.BindGraphicsPipeline(Pipeline_FixedColor_Fill_SID);
        renderer.Draw(3, 1, 0, 0);
        renderer.BindGraphicsPipeline(Pipeline_FixedColor_Line_SID);
        renderer.Draw(3, 1, 3, 0);

        // buffered color drawing
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanBufferManager.GetVertexBuffer(VertexBufferBuffered_SID)->GetVkBuffer() }, { 0 });
        renderer.BindGraphicsPipeline(Pipeline_BufferedColor_Fill_SID);
        renderer.Draw(3, 1, 0, 0);
        renderer.BindGraphicsPipeline(Pipeline_BufferedColor_Line_SID);
        renderer.Draw(3, 1, 3, 0);

        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}