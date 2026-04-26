#include "multiple_pipelines_vulkan_main_frame_listener.h"

#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto Pipeline_FixedColor_Fill_SID =    "Pipeline_FixedColor_Fill"_sid;
    static constexpr auto Pipeline_FixedColor_Line_SID =    "Pipeline_FixedColor_Line"_sid;
    static constexpr auto Pipeline_BufferedColor_Fill_SID = "Pipeline_BufferedColor_Fill"_sid;
    static constexpr auto Pipeline_BufferedColor_Line_SID = "Pipeline_BufferedColor_Line"_sid;

    static constexpr auto VertexShader_FixedColor_SID =         "FixedColor_vertex"_sid;
    static constexpr auto FragmentShader_FixedColor_SID =       "FixedColor_fragment"_sid;
    static constexpr auto VertexShader_BufferedColor_SID =      "BufferedColor_vertex"_sid;
    static constexpr auto FragmentShader_BufferedColor_SID =    "BufferedColor_fragment"_sid;

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


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _vertexBufferFixedColor(nullptr)
        , _vertexBufferBufferedColor(nullptr)
        , _device(VK_NULL_HANDLE)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MainFrameListener::~MainFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        const auto& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();
        _device = vulkanDevice.GetVkDevice();
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<FixedColorVertex> fixedColorVertices{
            // Top-left
            {-0.95f, -0.05f},
            {-0.05f, -0.05f},
            {-0.95f, -0.95f},

            // Bottom-left
            {-0.95f,  0.95f},
            {-0.05f,  0.95f},
            {-0.95f,  0.05f}
        };
        const auto fixedColorBufferSize = UInt32(fixedColorVertices.size() * sizeof(FixedColorVertex));

        const Vector<BufferedColorVertex> bufferedColorVertices{
            // Top-right
            { {0.05f, -0.05f}, {1.0f, 0.0f, 0.0f, 1.0f} },
            { {0.95f, -0.05f}, {0.0f, 1.0f, 0.0f, 1.0f} },
            { {0.05f, -0.95f}, {0.0f, 0.0f, 1.0f, 1.0f} },

            // Bottom-right
            { {0.05f,  0.95f}, {1.0f, 0.0f, 0.0f, 1.0f} },
            { {0.95f,  0.95f}, {0.0f, 1.0f, 0.0f, 1.0f} },
            { {0.05f,  0.05f}, {0.0f, 0.0f, 1.0f, 1.0f} }
        };
        const auto bufferedColorBufferSize = UInt32(bufferedColorVertices.size() * sizeof(BufferedColorVertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, fixedColorBufferSize + bufferedColorBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)fixedColorVertices.data(), fixedColorBufferSize);
        stagingBuffer.CopyToMappedMemory(fixedColorBufferSize, (char*)bufferedColorVertices.data(), bufferedColorBufferSize);
        stagingBuffer.Unmap("flush"_true);

        const auto fixedColorBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 0 }
        });
        _vertexBufferFixedColor.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, fixedColorBufferSize }));
        _vertexBufferFixedColor->AddLayout(fixedColorBufferLayout);

        const auto bufferColorBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 0 },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, 1 },
        });
        _vertexBufferBufferedColor.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, bufferedColorBufferSize }));
        _vertexBufferBufferedColor->AddLayout(bufferColorBufferLayout);

        {
            const auto copyCmd = renderer.CreateCommandBuffer();
            copyCmd.Begin();
            renderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBufferFixedColor.get(), 0, 0, fixedColorBufferSize);
            renderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBufferBufferedColor.get(), fixedColorBufferSize, 0, bufferedColorBufferSize);
            copyCmd.End();
            vulkanDevice.GetGraphicsQueue().SyncSubmit(copyCmd);
        }

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
        pipelineFixedColorFillParams.SetInputAssembly(VK_Primitive_TriangleList, "primitive restart"_false);
        pipelineFixedColorFillParams.SetPolygonMode(VK_Polygon_Fill);
        pipelineFixedColorFillParams.SetCulling(VK_Cull_Back, VK_FrontFace_CounterClockwise);
        pipelineFixedColorFillParams.SetDepthClamping(true);
        pipelineFixedColorFillParams.SetRasterizerDiscard(false);
        pipelineFixedColorFillParams.SetDepthBiasParameters("bias enabled"_false, 0.0f, 0.0f, 0.0f);
        pipelineFixedColorFillParams.SetDepthTest(true);
        pipelineFixedColorFillParams.SetDepthWrite(true);
        pipelineFixedColorFillParams.SetDepthComparison(VK_Compare_LessOrEqual);
        pipelineFixedColorFillParams.SetDepthBoundsTest(false);
        pipelineFixedColorFillParams.SetStencilTest(true);
        pipelineFixedColorFillParams.SetStencilStates(Graphics::VKPresets::StencilOpState_Disabled, Graphics::VKPresets::StencilOpState_Disabled);
        pipelineFixedColorFillParams.AddShaderStages(fixedColorShaderStages);
        pipelineFixedColorFillParams.AddVertexBufferAttributesBindings(*_vertexBufferFixedColor, 0);
        pipelineFixedColorFillParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        auto pipelineFixedColorLineParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineFixedColorFillParams, 
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::All
        );
        pipelineFixedColorLineParams.SetPolygonMode(VK_Polygon_Line);

        auto pipelineBufferedColorFillParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineFixedColorFillParams, 
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::ColorAttachmentInfos |
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::DynamicStates
        );
        pipelineBufferedColorFillParams.AddVertexBufferAttributesBindings(*_vertexBufferBufferedColor, 0);
        pipelineBufferedColorFillParams.AddShaderStages(bufferedColorShaderStages);

        auto pipelineBufferedColorLineParams = Graphics::VulkanGraphicsPipelineParameters::CopyFrom(pipelineBufferedColorFillParams,
            Graphics::VulkanGraphicsPipelineParameters::CopyParameters::All
        );
        pipelineBufferedColorLineParams.SetPolygonMode(VK_Polygon_Line);
        pipelineBufferedColorLineParams.SetLineRasterizationMode(VK_LineRasterization_RectangularSmooth);
        pipelineBufferedColorLineParams.SetLineWidth(8.0f);
        pipelineBufferedColorLineParams.SetLineStipple(true, 1, 1);

        vulkanDevice.AddGraphicsPipeline(Pipeline_FixedColor_Fill_SID, pipelineFixedColorFillParams);
        vulkanDevice.AddGraphicsPipeline(Pipeline_FixedColor_Line_SID, pipelineFixedColorLineParams);
        vulkanDevice.AddGraphicsPipeline(Pipeline_BufferedColor_Fill_SID, pipelineBufferedColorFillParams);
        vulkanDevice.AddGraphicsPipeline(Pipeline_BufferedColor_Line_SID, pipelineBufferedColorLineParams);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Finalize()
    {
        _vertexBufferFixedColor.reset();
        _vertexBufferBufferedColor.reset();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        // common setup for all pipelines
        renderer.BeginRendering(drawArea);
        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());

        // fixed color drawing
        renderer.BindVertexBuffers(0, { _vertexBufferFixedColor->GetVkBuffer() }, { VkDeviceSize{0} });
        renderer.BindGraphicsPipeline(Pipeline_FixedColor_Fill_SID);
        renderer.Draw(3, 1, 0, 0);
        renderer.BindGraphicsPipeline(Pipeline_FixedColor_Line_SID);
        renderer.Draw(3, 1, 3, 0);

        // buffered color drawing
        renderer.BindVertexBuffers(0, { _vertexBufferBufferedColor->GetVkBuffer() }, { VkDeviceSize{0} });
        renderer.BindGraphicsPipeline(Pipeline_BufferedColor_Fill_SID);
        renderer.Draw(3, 1, 0, 0);
        renderer.BindGraphicsPipeline(Pipeline_BufferedColor_Line_SID);
        renderer.Draw(3, 1, 3, 0);

        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}