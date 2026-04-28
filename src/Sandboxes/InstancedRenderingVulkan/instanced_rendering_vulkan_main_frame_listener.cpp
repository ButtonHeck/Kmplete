#include "instanced_rendering_vulkan_main_frame_listener.h"

#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto Pipeline_SID = "Pipeline"_sid;

    static constexpr auto VertexPositionIndex = 0;
    static constexpr auto VertexPositionInstancedIndex = 1;
    static constexpr auto VertexColorInstancedIndex = 2;

    static constexpr auto NumInstancesInRow = 9;


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


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _vertexBuffer(nullptr)
        , _vertexBufferPosInstanced(nullptr)
        , _vertexBufferColorsInstanced(nullptr)
        , _indexBuffer(nullptr)
        , _indexCount(0)
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

        const Vector<Vertex> vertices{
            { -0.1f,  0.1f },
            {  0.1f,  0.1f },
            { -0.1f, -0.1f }
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

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize + indexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)verticesInstanced.data(), vertexInstancedBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + vertexInstancedBufferSize, (char*)verticesColorsInstanced.data(), vertexColorsInstancedBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        const auto vertexBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionIndex }
        });
        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize }));
        _vertexBuffer->AddLayout(vertexBufferLayout);

        const auto vertexInstancedBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionInstancedIndex }
        }, "instanced"_true);
        _vertexBufferPosInstanced.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexInstancedBufferSize }));
        _vertexBufferPosInstanced->AddLayout(vertexInstancedBufferLayout);

        const auto vertexColorsInstancedBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, VertexColorInstancedIndex }
        }, "instanced"_true);
        _vertexBufferColorsInstanced.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexColorsInstancedBufferSize }));
        _vertexBufferColorsInstanced->AddLayout(vertexColorsInstancedBufferLayout);

        _indexBuffer.reset(vulkanBufferCreator.CreateIndexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, indexBufferSize }));

        {
            const auto copyCmd = renderer.CreateCommandBuffer();
            copyCmd.Begin();
            renderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBuffer.get(), 0, 0, vertexBufferSize);
            renderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBufferPosInstanced.get(), vertexBufferSize, 0, vertexInstancedBufferSize);
            renderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBufferColorsInstanced.get(), vertexBufferSize + vertexInstancedBufferSize, 0, vertexColorsInstancedBufferSize);
            renderer.CopyBuffer(copyCmd, stagingBuffer, *_indexBuffer.get(), vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize, 0, indexBufferSize);
            copyCmd.End();
            vulkanDevice.GetGraphicsQueue().SyncSubmit(copyCmd);
        }

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("instanced_rendering.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("instanced_rendering.frag.spv");
        const auto vertexShaderModule = vulkanDevice.CreateShaderModule(vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.CreateShaderModule(fragmentShaderPath);
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineParams.SetInputAssembly(VK_Primitive_TriangleList, "primitive restart"_false);
        pipelineParams.SetPolygonMode(VK_Polygon_Fill);
        pipelineParams.SetCulling(VK_Cull_Back, VK_FrontFace_CounterClockwise);
        pipelineParams.SetDepthClamping(true);
        pipelineParams.SetRasterizerDiscard(false);
        pipelineParams.SetDepthBiasParameters("bias enabled"_false, 0.0f, 0.0f, 0.0f);
        pipelineParams.SetDepthTest(true);
        pipelineParams.SetDepthWrite(true);
        pipelineParams.SetDepthComparison(VK_Compare_LessOrEqual);
        pipelineParams.SetDepthBoundsTest(false);
        pipelineParams.SetStencilTest(true);
        pipelineParams.SetStencilStates(Graphics::VKPresets::StencilOpState_Disabled, Graphics::VKPresets::StencilOpState_Disabled);
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBuffer, VertexPositionIndex);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBufferPosInstanced, VertexPositionInstancedIndex);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBufferColorsInstanced, VertexColorInstancedIndex);
        pipelineParams.AddVertexInputBindingsDivisors({{ VertexColorInstancedIndex, 2 }});
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        vulkanDevice.AddGraphicsPipeline(Pipeline_SID, pipelineParams);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Finalize()
    {
        _vertexBuffer.reset();
        _vertexBufferPosInstanced.reset();
        _vertexBufferColorsInstanced.reset();
        _indexBuffer.reset();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexPositionIndex, { _vertexBuffer->GetVkBuffer(), _vertexBufferPosInstanced->GetVkBuffer(), _vertexBufferColorsInstanced->GetVkBuffer() }, { 0, 0, 0 });
        renderer.BindIndexBuffer(*_indexBuffer.get());

        renderer.BeginRendering(drawArea);
        renderer.Draw(3, NumInstancesInRow, 0, 0);
        renderer.EndRendering();

        renderer.BeginRendering(drawArea, "clear previous"_false);
        renderer.BindVertexBuffers(VertexPositionInstancedIndex, { _vertexBufferPosInstanced->GetVkBuffer() }, { sizeof(Vertex) * NumInstancesInRow });
        renderer.DrawIndexed(3, NumInstancesInRow, 0, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}