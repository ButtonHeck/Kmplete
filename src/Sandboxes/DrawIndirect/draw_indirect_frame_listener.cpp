#include "draw_indirect_frame_listener.h"

#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture_attachment_manager.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "Pipeline"_sid;

    static constexpr auto VertexBufferBinding = 0;
    static constexpr auto InstanceBufferBinding = 1;

    static constexpr auto VertexPositionAttributeIndex = 0;
    static constexpr auto VertexInstancePositionAttributeIndex = 1;
    static constexpr auto VertexInstanceColorAttributeIndex = 2;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;


    namespace
    {
        struct Vertex
        {
            float position[2];
        };

        struct InstanceData
        {
            float position[2];
            float color[4];
        };
    }

    using namespace Graphics::VKBits;


    DrawIndirectFrameListener::DrawIndirectFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _vertexBuffer(nullptr)
        , _vertexInstanceBuffer(nullptr)
        , _indexBuffer(nullptr)
        , _indirectBuffer(nullptr)
        , _indexCount(0)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    DrawIndirectFrameListener::~DrawIndirectFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            { -0.1f,   0.1f },
            {  0.1f,   0.1f },
            { -0.09f, -0.1f },

            // offset by +0.1 on X axis
            { -0.09f, -0.1f },
            {  0.11f,  0.1f },
            {  0.11f, -0.1f },
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<InstanceData> instanceData{
            // Top row - 8 items
            { { -0.8f, -0.4f }, { 1.0f, 0.0f, 0.0f, 1.0f } /*red*/ },
            { { -0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f, 1.0f } /*green*/},
            { { -0.4f, -0.4f }, { 0.0f, 0.0f, 1.0f, 1.0f } /*blue*/},
            { { -0.2f, -0.4f }, { 1.0f, 1.0f, 1.0f, 1.0f } /*white*/},
            { {  0.0f, -0.4f }, { 1.0f, 1.0f, 0.0f, 1.0f } /*yellow*/},
            { {  0.2f, -0.4f }, { 1.0f, 0.0f, 1.0f, 1.0f } /*magenta*/},
            { {  0.4f, -0.4f }, { 0.0f, 1.0f, 1.0f, 1.0f } /*cyan*/},
            { {  0.6f, -0.4f }, { 0.5f, 0.5f, 0.5f, 1.0f } /*grey*/},

            // Bottom row - 8 items
            { { -0.8f, 0.4f }, { 1.0f, 0.0f, 0.0f, 1.0f } /*red*/ },
            { { -0.6f, 0.4f }, { 0.0f, 1.0f, 0.0f, 1.0f } /*green*/},
            { { -0.4f, 0.4f }, { 0.0f, 0.0f, 1.0f, 1.0f } /*blue*/},
            { { -0.2f, 0.4f }, { 1.0f, 1.0f, 1.0f, 1.0f } /*white*/},
            { {  0.0f, 0.4f }, { 1.0f, 1.0f, 0.0f, 1.0f } /*yellow*/},
            { {  0.2f, 0.4f }, { 1.0f, 0.0f, 1.0f, 1.0f } /*magenta*/},
            { {  0.4f, 0.4f }, { 0.0f, 1.0f, 1.0f, 1.0f } /*cyan*/},
            { {  0.6f, 0.4f }, { 0.5f, 0.5f, 0.5f, 1.0f } /*grey*/},
        };
        const auto instanceBufferSize = UInt32(instanceData.size() * sizeof(InstanceData));

        const Vector<UInt32> indices{ 0, 1, 2, 3, 4, 5 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);

        Vector<VkDrawIndirectCommand> drawIndirectCommands;
        VkDrawIndirectCommand command{};
        command.vertexCount = 3;
        command.instanceCount = 4;
        command.firstInstance = 0;
        command.firstVertex = 0;
        drawIndirectCommands.push_back(command);
        command.firstInstance = 4;
        command.firstVertex = 3;
        drawIndirectCommands.push_back(command);
        const auto drawInstancedBufferSize = UInt32(drawIndirectCommands.size() * sizeof(VkDrawIndirectCommand));

        Vector<VkDrawIndexedIndirectCommand> drawIndexedIndirectCommands;
        VkDrawIndexedIndirectCommand indexedCommand{};
        indexedCommand.firstIndex = 0;
        indexedCommand.firstInstance = 8;
        indexedCommand.indexCount = 3;
        indexedCommand.instanceCount = 4;
        indexedCommand.vertexOffset = 0;
        drawIndexedIndirectCommands.push_back(indexedCommand);
        indexedCommand.firstIndex = 3;
        indexedCommand.firstInstance = 12;
        drawIndexedIndirectCommands.push_back(indexedCommand);
        const auto drawIndexedInstanceBufferSize = UInt32(drawIndexedIndirectCommands.size() * sizeof(VkDrawIndexedIndirectCommand));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + instanceBufferSize + indexBufferSize + drawInstancedBufferSize + drawIndexedInstanceBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)instanceData.data(), instanceBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + instanceBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + instanceBufferSize + indexBufferSize, (char*)drawIndirectCommands.data(), drawInstancedBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + instanceBufferSize + indexBufferSize + drawInstancedBufferSize, (char*)drawIndexedIndirectCommands.data(), drawIndexedInstanceBufferSize);
        stagingBuffer.Unmap("flush"_true);

        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize }));
        _vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        _vertexInstanceBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, instanceBufferSize }));
        _vertexInstanceBuffer->AddLayout(Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexInstancePositionAttributeIndex },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, VertexInstanceColorAttributeIndex },
        }, "instanced"_true));

        _indexBuffer.reset(vulkanBufferCreator.CreateIndexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, indexBufferSize }));

        _indirectBuffer.reset(vulkanBufferCreator.CreateIndirectBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, drawInstancedBufferSize + drawIndexedInstanceBufferSize }));

        renderer.CopyBuffers(stagingBuffer, {
            { *_vertexBuffer.get(), 0, 0, vertexBufferSize },
            { *_vertexInstanceBuffer.get(), vertexBufferSize, 0, instanceBufferSize },
            { *_indexBuffer.get(), vertexBufferSize + instanceBufferSize, 0, indexBufferSize },
            { *_indirectBuffer.get(), vertexBufferSize + instanceBufferSize + indexBufferSize, 0, drawInstancedBufferSize },
            { *_indirectBuffer.get(), vertexBufferSize + instanceBufferSize + indexBufferSize + drawInstancedBufferSize, drawInstancedBufferSize, drawIndexedInstanceBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment, VK_ImageAspect_Color);
        textureAttachmentManager.AddTextureAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment, VK_ImageAspect_DepthStencil);

        vulkanDevice.GetPipelineManager().AddPipelineLayout(PipelineLayout_SID, {}, {});

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("draw_indirect.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("draw_indirect.frag.spv");
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
        pipelineParams.AddVertexBufferAttributesBindings(*_vertexInstanceBuffer, InstanceBufferBinding);
        pipelineParams.AddVertexInputBindingsDivisors({ 
            { InstanceBufferBinding, 1 }  // doesn't need to be set, just for debugging purposes
        });
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        vulkanDevice.GetPipelineManager().AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams);
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::_Finalize()
    {
        _vertexBuffer.reset();
        _vertexInstanceBuffer.reset();
        _indexBuffer.reset();
        _indirectBuffer.reset();
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { _vertexBuffer->GetVkBuffer(), _vertexInstanceBuffer->GetVkBuffer() }, { 0, 0 });
        renderer.BindIndexBuffer(*_indexBuffer.get());

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
        renderer.DrawIndirect(*_indirectBuffer.get(), 0, 2);
        renderer.DrawIndexedIndirect(*_indirectBuffer.get(), 2 * sizeof(VkDrawIndirectCommand), 2);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}