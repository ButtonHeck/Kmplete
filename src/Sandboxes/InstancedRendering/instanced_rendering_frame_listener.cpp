#include "instanced_rendering_frame_listener.h"

#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
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
        , _vertexBuffer(nullptr)
        , _vertexBufferPosInstanced(nullptr)
        , _vertexBufferColorsInstanced(nullptr)
        , _indexBuffer(nullptr)
        , _indexCount(0)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    InstancedRenderingFrameListener::~InstancedRenderingFrameListener()
    {
        _Finalize();
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

        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize }));
        _vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        _vertexBufferPosInstanced.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexInstancedBufferSize }));
        _vertexBufferPosInstanced->AddLayout(Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionInstancedAttributeIndex }
        }, "instanced"_true));

        _vertexBufferColorsInstanced.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexColorsInstancedBufferSize }));
        _vertexBufferColorsInstanced->AddLayout(Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, VertexColorInstancedAttributeIndex }
        }, "instanced"_true));

        _indexBuffer.reset(vulkanBufferCreator.CreateIndexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, indexBufferSize }));

        renderer.CopyBuffers(stagingBuffer, {
            { *_vertexBuffer.get(), 0, 0, vertexBufferSize },
            { *_vertexBufferPosInstanced.get(), vertexBufferSize, 0, vertexInstancedBufferSize },
            { *_vertexBufferColorsInstanced.get(), vertexBufferSize + vertexInstancedBufferSize, 0, vertexColorsInstancedBufferSize },
            { *_indexBuffer.get(), vertexBufferSize + vertexInstancedBufferSize + vertexColorsInstancedBufferSize, 0, indexBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        vulkanDevice.GetPipelineManager().AddPipelineLayout(PipelineLayout_SID, {}, {});

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
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBuffer, VertexBufferBinding);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBufferPosInstanced, InstancePositionBufferBinding);
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexBufferColorsInstanced, InstanceColorBufferBinding);
        pipelineParams.AddVertexInputBindingsDivisors({ { InstanceColorBufferBinding, 2 } }); // only color divisor set to 2, position divisor default 1 is ok
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams);
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::_Finalize()
    {
        _vertexBuffer.reset();
        _vertexBufferPosInstanced.reset();
        _vertexBufferColorsInstanced.reset();
        _indexBuffer.reset();
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { _vertexBuffer->GetVkBuffer(), _vertexBufferPosInstanced->GetVkBuffer(), _vertexBufferColorsInstanced->GetVkBuffer() }, { 0, 0, 0 });
        renderer.BindIndexBuffer(*_indexBuffer.get());

        renderer.BeginRendering(drawArea);
        renderer.Draw(3, NumInstancesInRow, 0, 0);
        renderer.EndRendering();

        renderer.BeginRendering(drawArea, "clear previous"_false);
        renderer.BindVertexBuffers(InstancePositionBufferBinding, { _vertexBufferPosInstanced->GetVkBuffer() }, { sizeof(Vertex) * NumInstancesInRow });
        renderer.DrawIndexed(3, NumInstancesInRow, 0, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}