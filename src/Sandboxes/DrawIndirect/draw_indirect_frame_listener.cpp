#include "draw_indirect_frame_listener.h"

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
    static constexpr auto InstanceBufferBinding = 1;

    static constexpr auto VertexPositionAttributeIndex = 0;
    static constexpr auto VertexInstancePositionAttributeIndex = 1;
    static constexpr auto VertexInstanceColorAttributeIndex = 2;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexShaderModule_SID = "vertex_shader"_sid;
    static constexpr auto FragmentShaderModule_SID = "fragment_shader"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffer"_sid;
    static constexpr auto VertexBufferInstanced_SID = "vertex_buffer_instanced"_sid;
    static constexpr auto IndexBuffer_SID = "index_buffer"_sid;
    static constexpr auto IndirectBuffer_SID = "indirect_buffer"_sid;


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
        , _indexCount(0)
    {
        _Initialize();
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
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
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

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + instanceBufferSize + indexBufferSize + drawInstancedBufferSize + drawIndexedInstanceBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)instanceData.data(), instanceBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + instanceBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + instanceBufferSize + indexBufferSize, (char*)drawIndirectCommands.data(), drawInstancedBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + instanceBufferSize + indexBufferSize + drawInstancedBufferSize, (char*)drawIndexedIndirectCommands.data(), drawIndexedInstanceBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
        auto vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        vulkanBufferManager.CreateVertexBuffer(VertexBufferInstanced_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, instanceBufferSize });
        auto vertexBufferInstanced = vulkanBufferManager.GetVertexBuffer(VertexBufferInstanced_SID);
        vertexBufferInstanced->AddLayout(Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexInstancePositionAttributeIndex },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, VertexInstanceColorAttributeIndex },
        }, "instanced"_true));

        vulkanBufferManager.CreateIndexBuffer(IndexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, indexBufferSize });
        auto indexBuffer = vulkanBufferManager.GetBuffer(IndexBuffer_SID);

        vulkanBufferManager.CreateIndirectBuffer(IndirectBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, drawInstancedBufferSize + drawIndexedInstanceBufferSize });
        auto indirectBuffer = vulkanBufferManager.GetBuffer(IndirectBuffer_SID);

        renderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize },
            { *vertexBufferInstanced, vertexBufferSize, 0, instanceBufferSize },
            { *indexBuffer, vertexBufferSize + instanceBufferSize, 0, indexBufferSize },
            { *indirectBuffer, vertexBufferSize + instanceBufferSize + indexBufferSize, 0, drawInstancedBufferSize },
            { *indirectBuffer, vertexBufferSize + instanceBufferSize + indexBufferSize + drawInstancedBufferSize, drawInstancedBufferSize, drawIndexedInstanceBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayout(PipelineLayout_SID, {});

        auto& shaderManager = vulkanDevice.GetShaderManager();
        shaderManager.AddShaderModules({
            { VertexShaderModule_SID, String(KMP_SANDBOX_RESOURCES_FOLDER).append("draw_indirect.vert.spv") },
            { FragmentShaderModule_SID, String(KMP_SANDBOX_RESOURCES_FOLDER).append("draw_indirect.frag.spv") }
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
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBufferInstanced_SID), InstanceBufferBinding);
        pipelineParams.AddVertexInputBindingsDivisors({ 
            { InstanceBufferBinding, 1 }  // doesn't need to be set, just for debugging purposes
        });
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "draw_indirect_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void DrawIndirectFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer(), vulkanBufferManager.GetVertexBuffer(VertexBufferInstanced_SID)->GetVkBuffer() }, { 0, 0 });
        renderer.BindIndexBuffer(*vulkanBufferManager.GetBuffer(IndexBuffer_SID));

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
        renderer.DrawIndirect(*vulkanBufferManager.GetBuffer(IndirectBuffer_SID), 0, 2);
        renderer.DrawIndexedIndirect(*vulkanBufferManager.GetBuffer(IndirectBuffer_SID), 2 * sizeof(VkDrawIndirectCommand), 2);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}