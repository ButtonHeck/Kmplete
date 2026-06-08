#include "multiple_pipelines_frame_listener.h"

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
        , _vertexBufferFixedColor(nullptr)
        , _vertexBufferBufferedColor(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MultiplePipelinesFrameListener::~MultiplePipelinesFrameListener()
    {
        _Finalize();
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
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
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

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, fixedColorBufferSize + bufferedColorBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)fixedColorVertices.data(), fixedColorBufferSize);
        stagingBuffer.CopyToMappedMemory(fixedColorBufferSize, (char*)bufferedColorVertices.data(), bufferedColorBufferSize);
        stagingBuffer.Unmap("flush"_true);

        _vertexBufferFixedColor.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, fixedColorBufferSize }));
        _vertexBufferFixedColor->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 0 }
        });

        _vertexBufferBufferedColor.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, bufferedColorBufferSize }));
        _vertexBufferBufferedColor->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 0 },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, 1 },
        });

        renderer.CopyBuffers(stagingBuffer, {
            { *_vertexBufferFixedColor.get(), 0, 0, fixedColorBufferSize },
            { *_vertexBufferBufferedColor.get(), fixedColorBufferSize, 0, bufferedColorBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::_InitializePipelines(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment, VK_ImageAspect_Color);
        textureAttachmentManager.AddTextureAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment, VK_ImageAspect_DepthStencil);

        vulkanDevice.GetPipelineManager().AddPipelineLayout(PipelineLayout_SID, {}, {});

        const auto fixedColorVertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("multiple_pipelines_fixed_color.vert.spv");
        const auto fixedColorFragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("multiple_pipelines_fixed_color.frag.spv");
        const auto fixedColorVertexShaderModule = vulkanDevice.CreateShaderModule(fixedColorVertexShaderPath);
        const auto fixedColorFragmentShaderModule = vulkanDevice.CreateShaderModule(fixedColorFragmentShaderPath);
        const auto fixedColorShaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            fixedColorVertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fixedColorFragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        const auto bufferedColorVertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("multiple_pipelines_buffered_color.vert.spv");
        const auto bufferedColorFragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("multiple_pipelines_buffered_color.frag.spv");
        const auto bufferedColorVertexShaderModule = vulkanDevice.CreateShaderModule(bufferedColorVertexShaderPath);
        const auto bufferedColorFragmentShaderModule = vulkanDevice.CreateShaderModule(bufferedColorFragmentShaderPath);
        const auto bufferedColorShaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            bufferedColorVertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            bufferedColorFragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelineFixedColorFillParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineFixedColorFillParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineFixedColorFillParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineFixedColorFillParams.AddShaderStages(fixedColorShaderStages);
        pipelineFixedColorFillParams.AddVertexBufferAttributesBindings(*_vertexBufferFixedColor, VertexBufferBinding);
        pipelineFixedColorFillParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        auto pipelineFixedColorLineParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineFixedColorFillParams, 
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::All
        );
        pipelineFixedColorLineParams.SetPolygonMode(VK_Polygon_Line);

        auto pipelineBufferedColorFillParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineFixedColorFillParams, 
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::ColorAttachmentInfos |
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::DynamicStates
        );
        pipelineBufferedColorFillParams.AddVertexBufferAttributesBindings(*_vertexBufferBufferedColor, VertexBufferBinding);
        pipelineBufferedColorFillParams.AddShaderStages(bufferedColorShaderStages);

        auto pipelineBufferedColorLineParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineBufferedColorFillParams,
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::All
        );
        pipelineBufferedColorLineParams.SetPolygonMode(VK_Polygon_Line);
        pipelineBufferedColorLineParams.SetLineRasterizationMode(VK_LineRasterization_RectangularSmooth);
        pipelineBufferedColorLineParams.SetLineWidth(8.0f);
        pipelineBufferedColorLineParams.SetLineStipple(true, 1, 1);

        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_FixedColor_Fill_SID, PipelineLayout_SID, pipelineFixedColorFillParams);
        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_FixedColor_Line_SID, PipelineLayout_SID, pipelineFixedColorLineParams);
        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_BufferedColor_Fill_SID, PipelineLayout_SID, pipelineBufferedColorFillParams);
        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_BufferedColor_Line_SID, PipelineLayout_SID, pipelineBufferedColorLineParams);
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::_Finalize()
    {
        _vertexBufferFixedColor.reset();
        _vertexBufferBufferedColor.reset();
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
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
        renderer.BindVertexBuffers(VertexBufferBinding, { _vertexBufferFixedColor->GetVkBuffer() }, { 0 });
        renderer.BindGraphicsPipeline(Pipeline_FixedColor_Fill_SID);
        renderer.Draw(3, 1, 0, 0);
        renderer.BindGraphicsPipeline(Pipeline_FixedColor_Line_SID);
        renderer.Draw(3, 1, 3, 0);

        // buffered color drawing
        renderer.BindVertexBuffers(VertexBufferBinding, { _vertexBufferBufferedColor->GetVkBuffer() }, { 0 });
        renderer.BindGraphicsPipeline(Pipeline_BufferedColor_Fill_SID);
        renderer.Draw(3, 1, 0, 0);
        renderer.BindGraphicsPipeline(Pipeline_BufferedColor_Line_SID);
        renderer.Draw(3, 1, 3, 0);

        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}