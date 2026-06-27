#include "post_processing_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/named_bool.h"


namespace Kmplete
{
    static constexpr auto PostProcessingDSLayout_SID = "PostProcessingDSLayout"_sid;
    static constexpr auto PostProcessingSet_SID = "PostProcessingSet"_sid;

    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "Pipeline"_sid;
    static constexpr auto PipelineResolve_SID = "PipelineResolve"_sid;

    static constexpr auto VertexBufferBinding = 0;

    static constexpr auto VertexPositionAttributeIndex = 0;
    static constexpr auto VertexColorAttributeIndex = 1;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;
    static constexpr auto ColorAttachmentResolve = "color_attachment_resolve"_sid;
    static constexpr auto DepthStencilAttachmentResolve = "depth_attachment_resolve"_sid;

    static constexpr auto TextureBindingIndex = 0;
    static constexpr auto SamplerBindingIndex = 1;
    static constexpr auto UniformBufferIndex = 2;

    static constexpr auto VertexShaderModule_SID = "vertex_shader"_sid;
    static constexpr auto FragmentShaderModule_SID = "fragment_shader"_sid;
    static constexpr auto VertexShaderResolveModule_SID = "vertex_shader_resolve"_sid;
    static constexpr auto FragmentShaderResolveModule_SID = "fragment_shader_resolve"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffer"_sid;
    static constexpr auto VertexBufferResolve_SID = "vertex_buffer_resolve"_sid;
    static constexpr auto UniformBuffersResolve_SID = "uniform_buffers_resolve"_sid;


    namespace
    {
        struct Vertex
        {
            float position[2];
            float color[4];
        };

        struct VertexResolve
        {
            float position[2];
        };
    }

    using namespace Graphics::VKBits;


    PostProcessingFrameListener::PostProcessingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        const auto& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();

        _InitializeBuffers(vulkanDevice);
        _InitializeUniformBuffers(vulkanDevice, vulkanContext);
        _InitializePipeline(vulkanDevice, vulkanContext);
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& renderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            { { -0.9f,  0.9f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { {  0.9f,  0.9f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { {  0.0f, -0.9f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<VertexResolve> verticesResolve{
            { -1.0f,  1.0f },
            {  1.0f,  1.0f },
            { -1.0f, -1.0f },

            { -1.0f, -1.0f },
            {  1.0f,  1.0f },
            {  1.0f, -1.0f }
        };
        const auto verticesResolveBufferSize = UInt32(verticesResolve.size() * sizeof(VertexResolve));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + verticesResolveBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)verticesResolve.data(), verticesResolveBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
        auto vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float4, VertexColorAttributeIndex }
        });

        vulkanBufferManager.CreateVertexBuffer(VertexBufferResolve_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, verticesResolveBufferSize });
        auto vertexBufferResolve = vulkanBufferManager.GetVertexBuffer(VertexBufferResolve_SID);
        vertexBufferResolve->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex }
        });

        renderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize },
            { *vertexBufferResolve, vertexBufferSize, 0, verticesResolveBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& samplersStorage = vulkanDevice.GetSamplersStorage();

        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment, VK_ImageAspect_Color);
        textureAttachmentManager.AddTextureAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment, VK_ImageAspect_DepthStencil);
        textureAttachmentManager.AddTextureAttachment(ColorAttachmentResolve, vulkanContext.surfaceFormat.format, VK_ImageUsage_Sampled | VK_ImageUsage_ColorAttachment, VK_ImageAspect_Color);
        textureAttachmentManager.AddTextureAttachment(DepthStencilAttachmentResolve, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment, VK_ImageAspect_DepthStencil);

        VkDescriptorSetLayoutBinding textureLayoutBinding{ TextureBindingIndex, VK_DescriptorType_SampledImage, 1, VK_ShaderStage_Fragment };
        VkDescriptorSetLayoutBinding samplerLayoutBinding{ SamplerBindingIndex, VK_DescriptorType_Sampler, 1, VK_ShaderStage_Fragment };
        VkDescriptorSetLayoutBinding uboLayoutBinding{ UniformBufferIndex, VK_DescriptorType_UniformBuffer, 1, VK_ShaderStage_Fragment };
        const auto postProcessingUniformsLayout = descriptorSetManager.AddDescriptorSetLayout(PostProcessingDSLayout_SID, { textureLayoutBinding, samplerLayoutBinding, uboLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(postProcessingUniformsLayout, PostProcessingSet_SID, 1, "per frame"_true);

        vulkanBufferManager.CreateUniformBuffer(UniformBuffersResolve_SID, { 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, sizeof(float) * 2 }, "per frame"_true);
        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            auto uniformBuffer = vulkanBufferManager.GetBuffer(UniformBuffersResolve_SID, i);
            uniformBuffer->Map();

            descriptorSetManager.SetSampledImageDescriptor(PostProcessingSet_SID, 0, "per frame"_true, i, textureAttachmentManager.GetTextureAttachment(ColorAttachmentResolve)->get().GetVkImageView(), TextureBindingIndex);
            descriptorSetManager.SetSamplerDescriptor(PostProcessingSet_SID, 0, "per frame"_true, i, samplersStorage.GetSampler(Graphics::SamplerDefaultNearestSid), SamplerBindingIndex);
            descriptorSetManager.SetUniformBufferDescriptor(PostProcessingSet_SID, 0, "per frame"_true, i, *uniformBuffer, uniformBuffer->GetSize(), 0, UniformBufferIndex);
        }
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayout(PipelineLayout_SID, {
            vulkanDevice.GetDescriptorSetManager().GetDescriptorSetLayout(PostProcessingDSLayout_SID)
        }, {});

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("post_processing_pre.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("post_processing_pre.frag.spv");
        const auto vertexShaderModule = vulkanDevice.GetShaderManager().AddShaderModule(VertexShaderModule_SID, vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.GetShaderManager().AddShaderModule(FragmentShaderModule_SID, fragmentShaderPath);
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.value().get().GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderModule.value().get().GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_AlphaBlending);
        pipelineParams.AddShaderStages(shaderStages);

        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID), VertexBufferBinding);
        pipelineParams.AddDynamicState(VK_Dynamic_Viewport);
        pipelineParams.AddDynamicState(VK_Dynamic_Scissor);
        pipelineParams.AddDynamicState(VK_Dynamic_RasterizationSamples);

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "post_processing_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::Render()
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
        renderer.Draw(3, 1, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}