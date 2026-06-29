#include "post_processing_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/ImGui/implementation_glfw_vulkan.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Event/event_queue.h"
#include "Kmplete/Assets/assets_manager.h"


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
    static constexpr auto VertexTexCoordAttributeIndex = 1;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto ColorAttachmentResolve = "color_attachment_resolve"_sid;

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
            float texCoord[2];
        };
    }

    using namespace Graphics::VKBits;


    PostProcessingFrameListener::PostProcessingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _imguiImpl(nullptr)
        , _multisamplingChangeHandler(_eventDispatcher, KMP_BIND(PostProcessingFrameListener::_OnMultisamplingChangeEvent))
        , _windowContentScaleHandler(_eventDispatcher, KMP_BIND(PostProcessingFrameListener::_OnWindowContentScaleEvent))
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeBuffers(vulkanDevice);
        _InitializeUniformBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
        _InitializeImGui(_mainWindow.GetDPIScale());
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
            { { -1.0f,  1.0f }, { 0.0f, 1.0f } },
            { {  1.0f,  1.0f }, { 1.0f, 1.0f } },
            { { -1.0f, -1.0f }, { 0.0f, 0.0f } },

            { { -1.0f, -1.0f }, { 0.0f, 0.0f } },
            { {  1.0f,  1.0f }, { 1.0f, 1.0f } },
            { {  1.0f, -1.0f }, { 1.0f, 0.0f } }
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
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexTexCoordAttributeIndex }
        });

        renderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize },
            { *vertexBufferResolve, vertexBufferSize, 0, verticesResolveBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& samplersStorage = vulkanDevice.GetSamplersStorage();

        VkDescriptorSetLayoutBinding textureLayoutBinding{ TextureBindingIndex, VK_DescriptorType_SampledImage, 1, VK_ShaderStage_Fragment };
        VkDescriptorSetLayoutBinding samplerLayoutBinding{ SamplerBindingIndex, VK_DescriptorType_Sampler, 1, VK_ShaderStage_Fragment };
        VkDescriptorSetLayoutBinding uboLayoutBinding{ UniformBufferIndex, VK_DescriptorType_UniformBuffer, 1, VK_ShaderStage_Fragment };
        const auto postProcessingUniformsLayout = descriptorSetManager.AddDescriptorSetLayout(PostProcessingDSLayout_SID, { textureLayoutBinding, samplerLayoutBinding, uboLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(postProcessingUniformsLayout, PostProcessingSet_SID, 1, "per frame"_true);

        vulkanBufferManager.CreateUniformBuffer(UniformBuffersResolve_SID, { 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, sizeof(VkExtent2D) }, "per frame"_true);
        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            auto uniformBuffer = vulkanBufferManager.GetBuffer(UniformBuffersResolve_SID, i);
            uniformBuffer->Map();

            descriptorSetManager.SetSamplerDescriptor(PostProcessingSet_SID, 0, "per frame"_true, i, samplersStorage.GetSampler(Graphics::SamplerDefaultNearestSid), SamplerBindingIndex);
            descriptorSetManager.SetUniformBufferDescriptor(PostProcessingSet_SID, 0, "per frame"_true, i, *uniformBuffer, uniformBuffer->GetSize(), 0, UniformBufferIndex);
        }
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        const auto attachmentsExtent = Graphics::VKUtils::Extent2Dto3D(vulkanDevice.GetCurrentExtent());
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_Sampled | VK_ImageUsage_ColorAttachment);
        textureAttachmentManager.AddTextureColorAttachment(ColorAttachmentResolve, vulkanContext.surfaceFormat.format, attachmentsExtent, VK_SampleCount_1, VK_ImageUsage_Sampled | VK_ImageUsage_ColorAttachment, "fixed samples"_true);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayoutWithSetsSids(PipelineLayout_SID, { PostProcessingDSLayout_SID }, {});

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


        const auto vertexPostShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("post_processing_post.vert.spv");
        const auto fragmentPostShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("post_processing_post.frag.spv");
        const auto vertexShaderPostModule = vulkanDevice.GetShaderManager().AddShaderModule(VertexShaderResolveModule_SID, vertexPostShaderPath);
        const auto fragmentShaderPostModule = vulkanDevice.GetShaderManager().AddShaderModule(FragmentShaderResolveModule_SID, fragmentPostShaderPath);
        const auto shaderPostStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderPostModule.value().get().GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderPostModule.value().get().GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelinePostParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelinePostParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelinePostParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_AlphaBlending);
        pipelinePostParams.AddShaderStages(shaderPostStages);
        pipelinePostParams.AddVertexBufferAttributesBindings(*vulkanBufferManager.GetVertexBuffer(VertexBufferResolve_SID), VertexBufferBinding);
        pipelinePostParams.AddDynamicState(VK_Dynamic_Viewport);
        pipelinePostParams.AddDynamicState(VK_Dynamic_Scissor);
        pipelinePostParams.AddDynamicState(VK_Dynamic_RasterizationSamples);

        pipelineManager.AddGraphicsPipeline(PipelineResolve_SID, PipelineLayout_SID, pipelinePostParams, ApplicationContext::GetApplicationDataPath() / "post_processing_post_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_InitializeImGui(float dpiScale)
    {
        ImGuiUtils::Context* context = nullptr;
        if (_graphicsBackend.GetType() == Graphics::GraphicsBackendType::Vulkan)
        {
            const auto& vulkanBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
            auto& physicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
            auto& logicalDevice = dynamic_cast<Graphics::VulkanLogicalDevice&>(physicalDevice.GetLogicalDevice());

            logicalDevice.GetDescriptorSetManager().AllocateAuxDescriptorPool("ImGui_Pool"_sid, 100, {
                { VK_DescriptorType_Sampler, 100 },
                { VK_DescriptorType_CombinedImageSampler, 100 },
                { VK_DescriptorType_SampledImage, 100 },
                { VK_DescriptorType_StorageImage, 100 },
                { VK_DescriptorType_UniformTexelBuffer, 100 },
                { VK_DescriptorType_StorageTexelBuffer, 100 },
                { VK_DescriptorType_UniformBuffer, 100 },
                { VK_DescriptorType_StorageBuffer, 100 },
                { VK_DescriptorType_UniformBufferDynamic, 100 },
                { VK_DescriptorType_StorageBufferDynamic, 100 },
                { VK_DescriptorType_InputAttachment, 100 }
                });

            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = vulkanBackend.GetVkInstance();
            initInfo.PhysicalDevice = physicalDevice.GetVkPhysicalDevice();
            initInfo.Device = logicalDevice.GetVkDevice();
            initInfo.QueueFamily = physicalDevice.GetVulkanContext().graphicsFamilyIndex;
            initInfo.Queue = logicalDevice.GetGraphicsQueue().GetVkQueue();
            initInfo.PipelineCache = VK_NULL_HANDLE;
            initInfo.DescriptorPool = logicalDevice.GetDescriptorSetManager().GetAuxDescriptorPool("ImGui_Pool"_sid);
            initInfo.Allocator = VK_NULL_HANDLE;
            initInfo.MinImageCount = Graphics::NumConcurrentFrames;
            initInfo.ImageCount = Graphics::NumConcurrentFrames;
            initInfo.CheckVkResultFn = nullptr;
            initInfo.UseDynamicRendering = true;
            initInfo.MSAASamples = VK_SampleCount_1; // always draw on single sampled attachment
            initInfo.PipelineRenderingCreateInfo = Graphics::VKUtils::InitVkPipelineRenderingCreateInfoKHR();
            initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
            initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &physicalDevice.GetVulkanContext().surfaceFormat.format;
            initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            initInfo.PipelineRenderingCreateInfo.stencilAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            context = new ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), Graphics::GraphicsBackendTypeToString(_graphicsBackend.GetType()), "docking"_false, "viewports"_true, dpiScale, initInfo);
            context->configName = "PostProcessingSandbox_imgui.ini";
        }
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(context));

        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale(), 15);
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::Render()
    {
        _RenderTriangle();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_RenderTriangle()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto& currentExtent = vulkanDevice.GetCurrentExtent();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, currentExtent };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };
        const auto currentBufferIndex = vulkanGraphicsBackend.GetCurrentBufferIndex();
        const auto currentMSAA = vulkanDevice.GetMultisampling();
        const auto colorAttachmentMS = vulkanTextureAttachmentManager.GetTextureAttachment(MS_ColorAttachment);
        const auto colorAttachmentResolve = vulkanTextureAttachmentManager.GetTextureAttachment(ColorAttachmentResolve);

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);

        // 1.1 Start with the offscreen pipeline rendering
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer() }, { 0 });
        renderer.SetRasterizationSamples(currentMSAA);

        // 1.2 Prepare color attachments and insert memory barriers for writing to attachment (and use single sampled resolve texture when MSAA > 1)
        VkRenderingAttachmentInfo colorAttachmentInfo{};
        auto colorMemoryBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_ColorAttachment_PrepareWriting;
        if (currentMSAA == VK_SampleCount_1)
        {
            renderer.InsertImageMemoryBarrier(colorAttachmentMS, colorMemoryBarrierParameters);
            colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
                Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
                MS_ColorAttachment, 0ULL, VK_Resolve_None, VK_ImageLayout_ColorAttachmentOptimal
            );
        }
        else
        {
            renderer.InsertImageMemoryBarrier(colorAttachmentResolve, colorMemoryBarrierParameters);
            colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
                Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
                MS_ColorAttachment, ColorAttachmentResolve, VK_Resolve_Average, VK_ImageLayout_ColorAttachmentOptimal
            );
        }

        // 1.3 Render RGB triangle to the attachment
        renderer.BeginRendering(drawArea, { colorAttachmentInfo });
        renderer.Draw(3, 1, 0, 0);
        renderer.EndRendering();


        // 2.1 Bind actual screen rendering pipeline
        renderer.BindGraphicsPipeline(PipelineResolve_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBufferResolve_SID)->GetVkBuffer() }, { 0 });
        renderer.SetRasterizationSamples(VK_SampleCount_1); // either single sampled image or a swapchain image is used here - always sample count 1

        // 2.2 Prepare color attachments and insert memory barriers for reading previously written image from shader (use resolve texture when MSAA > 1)
        VkRenderingAttachmentInfo colorAttachmentResolveInfo{};
        auto colorShaderReadMemoryBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_ColorAttachment_PrepareReadFromShader;
        if (currentMSAA == VK_SampleCount_1)
        {
            renderer.InsertImageMemoryBarrier(colorAttachmentMS, colorShaderReadMemoryBarrierParameters);
            colorAttachmentResolveInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
                Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
                MS_ColorAttachment, 0ULL, VK_Resolve_Average, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true
            );
            descriptorSetManager.SetSampledImageDescriptor(PostProcessingSet_SID, 0, "per frame"_true, currentBufferIndex, colorAttachmentMS->get().GetVkImageView(), TextureBindingIndex);
        }
        else
        {
            renderer.InsertImageMemoryBarrier(colorAttachmentResolve, colorShaderReadMemoryBarrierParameters);
            colorAttachmentResolveInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
                Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
                ColorAttachmentResolve, 0ULL, VK_Resolve_None, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true
            );
            descriptorSetManager.SetSampledImageDescriptor(PostProcessingSet_SID, 0, "per frame"_true, currentBufferIndex, colorAttachmentResolve->get().GetVkImageView(), TextureBindingIndex);
        }

        // 2.3 Update uniform buffer data
        Vector<float> uboData = { float(currentExtent.width), float(currentExtent.height) };
        vulkanBufferManager.GetBuffer(UniformBuffersResolve_SID, currentBufferIndex)->CopyToMappedMemory(0, uboData.data(), sizeof(VkExtent2D));

        // 2.4 Render screen quad with an offscreen attachment as texture
        renderer.BeginRendering(drawArea, { colorAttachmentResolveInfo });
        renderer.BindDescriptorSets(PipelineLayout_SID, 0, {
            descriptorSetManager.GetDescriptorSet(PostProcessingSet_SID, 0, "per frame"_true)
        });
        renderer.Draw(6, 1, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_RenderImGui()
    {
        auto& vulkanLogicalDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        const auto& vulkanTextureAttachmentManager = vulkanLogicalDevice.GetTextureAttachmentManager();
        auto commandBuffer = vulkanLogicalDevice.GetRenderer().GetCurrentCommandBuffer();
        auto* vulkanImGuiUtils = dynamic_cast<ImGuiUtils::ImGuiImplementationGlfwVulkan*>(_imguiImpl.get());
        const auto& renderer = vulkanLogicalDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanLogicalDevice.GetCurrentExtent() };

        _imguiImpl->NewFrame();

        static constexpr auto applicationWindowFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

        const auto imguiScale = ImGui::GetMainViewport()->DpiScale;
        const auto imguiViewportPos = ImGui::GetMainViewport()->WorkPos;
        const auto imguiWindowPos = ImVec2{ imguiViewportPos.x + 16, imguiViewportPos.y + 16 };
        ImGui::SetNextWindowPos(imguiWindowPos, ImGuiCond_Always);
        ImGui::Begin("PostProcessing sandbox", nullptr, applicationWindowFlags);
        ImGui::SetWindowSize(ImVec2(120 * imguiScale, 100 * imguiScale));

        if (ImGui::Button("Multisampling 1"))
        {
            _SetMultisampling(1);
        }
        if (ImGui::Button("Multisampling 2"))
        {
            _SetMultisampling(2);
        }
        if (ImGui::Button("Multisampling 4"))
        {
            _SetMultisampling(4);
        }

        ImGui::End();

        const auto currentMSAA = vulkanLogicalDevice.GetMultisampling();

        VkRenderingAttachmentInfo colorAttachmentInfo{};
        if (currentMSAA == VK_SampleCount_1)
        {
            colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
                Graphics::VKPresets::RenderingAttachmentInfo_Color_LoadStore,
                MS_ColorAttachment, 0ULL, VK_Resolve_Average, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true
            );
        }
        else
        {
            colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
                Graphics::VKPresets::RenderingAttachmentInfo_Color_LoadStore,
                ColorAttachmentResolve, 0ULL, VK_Resolve_None, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true
            );
        }

        renderer.BeginRendering(drawArea, { colorAttachmentInfo } );
        vulkanImGuiUtils->SetCommandBuffer(commandBuffer);
        vulkanImGuiUtils->Render();
        renderer.EndRendering();

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------

    void PostProcessingFrameListener::_SetMultisampling(UInt32 samples)
    {
        Events::QueueEvent(CreateUPtr<Events::MultisamplingChangeEvent>(samples));
    }
    //--------------------------------------------------------------------------

    bool PostProcessingFrameListener::_OnMultisamplingChangeEvent(Events::MultisamplingChangeEvent& evt)
    {
        _graphicsBackend.SetMultisampling(evt.msaaSamples);
        _imguiImpl.reset();
        _InitializeImGui(_mainWindow.GetDPIScale());
        return true;
    }
    //--------------------------------------------------------------------------

    bool PostProcessingFrameListener::_OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event)
    {
        const auto scale = event.GetScale();

        _imguiImpl.reset();
        _InitializeImGui(scale);

        return true;
    }
    //--------------------------------------------------------------------------
}