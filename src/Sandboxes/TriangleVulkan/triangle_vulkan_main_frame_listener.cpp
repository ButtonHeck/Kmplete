#include "triangle_vulkan_main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/ImGui/implementation_glfw_vulkan.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Event/event_queue.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace
    {
        struct Vertex
        {
            float position[2];
            float color[3];
        };
        //--------------------------------------------------------------------------
    }


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _vertexBuffer(nullptr)
        , _indexBuffer(nullptr)
        , _indexCount(0)
        , _device(VK_NULL_HANDLE)
        , _descriptorSetLayout(VK_NULL_HANDLE)
        , _commandBuffer(VK_NULL_HANDLE)
        , _imguiImpl(nullptr)
        , _assetsManager(assetsManager)
        , _multisamplingChangeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMultisamplingChangeEvent))
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
        _InitializeTriangle();
        _InitializeImGui(_mainWindow.GetDPIScale());
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_InitializeTriangle()
    {
        Graphics::VulkanPhysicalDevice& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        Graphics::VulkanLogicalDevice& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        const Graphics::VulkanContext& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();
        _device = vulkanDevice.GetVkDevice();

        const Vector<Vertex> vertices{
            {{ 0.95f,  0.95f}, {1.0f, 0.0f, 0.0f}},
            {{-0.95f,  0.97f}, {0.0f, 1.0f, 0.0f}},
            {{-0.92f, -0.95f}, {0.0f, 0.0f, 1.0f}}
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));


        const Vector<UInt32> indices{ 0, 1, 2 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);


        Graphics::VulkanBuffer stagingBuffer = vulkanDevice.CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vertexBufferSize + indexBufferSize);


        auto result = stagingBuffer.Map();
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to map texture buffer");
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)indices.data(), indexBufferSize);
        result = stagingBuffer.Flush();
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to flush texture buffer");
        stagingBuffer.Unmap();


        const auto vertexBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{Graphics::ShaderDataType::Float2, 0},
            Graphics::BufferElement{Graphics::ShaderDataType::Float3, 1}
        });
        _vertexBuffer.reset(vulkanDevice.CreateVertexBufferPtr(VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize));
        _vertexBuffer->AddLayout(vertexBufferLayout);


        _indexBuffer.reset(vulkanDevice.CreateBufferPtr(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize));


        {
            Graphics::VulkanCommandBuffer copyCmd = vulkanDevice.CreateCommandBuffer();
            VkCommandBuffer commandBuffer = copyCmd.GetVkCommandBuffer();
            copyCmd.Begin();

            VkBufferCopy copyRegion{};
            copyRegion.size = vertexBufferSize;
            vkCmdCopyBuffer(commandBuffer, stagingBuffer.GetVkBuffer(), _vertexBuffer->GetVkBuffer(), 1, &copyRegion);
            copyRegion.size = indexBufferSize;
            copyRegion.srcOffset = vertexBufferSize;
            vkCmdCopyBuffer(commandBuffer, stagingBuffer.GetVkBuffer(), _indexBuffer->GetVkBuffer(), 1, &copyRegion);
            copyCmd.End();


            Graphics::VulkanFence fence = vulkanDevice.CreateFence(false);
            vulkanDevice.GetGraphicsQueue().Submit(copyCmd, fence.GetVkFence());
            fence.Wait();
        }


        auto descriptorSetLayoutCI = Graphics::VulkanUtils::InitVkDescriptorSetLayoutCreateInfo();
        descriptorSetLayoutCI.bindingCount = 0;
        descriptorSetLayoutCI.pBindings = nullptr;
        result = vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutCI, nullptr, &_descriptorSetLayout);
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to create descriptor set layout");

        auto& pipeline = vulkanDevice.AddGraphicsPipeline("VulkanTriangle"_sid);
        pipeline.AddDescriptorSetLayout(_descriptorSetLayout);
        pipeline.SetupInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
        pipeline.SetupPolygonMode(VK_POLYGON_MODE_FILL);
        pipeline.SetupCulling(VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        pipeline.SetupDepthClamping(false);
        pipeline.SetupRasterizerDiscard(false);
        pipeline.SetupDepthBiasParameters(false, 0.0f, 0.0f, 0.0f);

        VkPipelineColorBlendAttachmentState blendAttachmentState{};
        blendAttachmentState.colorWriteMask = 0xf;
        blendAttachmentState.blendEnable = VK_FALSE;
        pipeline.AddColorBlendAttachment(blendAttachmentState);

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT).AddDynamicState(VK_DYNAMIC_STATE_SCISSOR).AddDynamicState(VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT);
        pipeline.SetupDepthTest(true);
        pipeline.SetupDepthWrite(true);
        pipeline.SetupDepthComparison(VK_COMPARE_OP_LESS_OR_EQUAL);
        pipeline.SetupDepthBoundsTest(false, 0.0f, 1.0f);
        pipeline.SetupStencilTest(false);
        pipeline.SetupMultisamplingSamples(VK_SAMPLE_COUNT_1_BIT);
        pipeline.SetupRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipeline.AddRenderingColorAttachment(vulkanContext.surfaceFormat.format);

        VkStencilOpState back{};
        back.failOp = VK_STENCIL_OP_KEEP;
        back.passOp = VK_STENCIL_OP_KEEP;
        back.compareOp = VK_COMPARE_OP_ALWAYS;
        pipeline.SetupStencilStates(back, back);

        auto [inputDescriptions, attributeDescriptions] = _vertexBuffer->GetBindingsDescriptions(0);
        pipeline.AddVertexInputBindings(std::move(inputDescriptions));
        pipeline.AddVertexAttributesDescriptions(std::move(attributeDescriptions));

        const auto vertexShader = vulkanDevice.CreateShader(String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.vert.spv"));
        const auto fragmentShader = vulkanDevice.CreateShader(String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.frag.spv"));
        auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShader.GetShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "main"),
            fragmentShader.GetShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "main")
        };
        pipeline.AddShaderStages(std::move(shaderStages));

        pipeline.Build();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_InitializeImGui(float dpiScale)
    {
        ImGuiUtils::Context* context = nullptr;
        if (_graphicsBackend.GetType() == Graphics::GraphicsBackendType::Vulkan)
        {
            const auto& vulkanBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
            const auto& physicalDevice = dynamic_cast<const Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
            const auto& logicalDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(physicalDevice.GetLogicalDevice());

            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = vulkanBackend.GetVkInstance();
            initInfo.PhysicalDevice = physicalDevice.GetVkPhysicalDevice();
            initInfo.Device = logicalDevice.GetVkDevice();
            initInfo.QueueFamily = physicalDevice.GetVulkanContext().graphicsFamilyIndex;
            initInfo.Queue = logicalDevice.GetGraphicsQueue().GetVkQueue();
            initInfo.PipelineCache = VK_NULL_HANDLE;
            initInfo.DescriptorPool = logicalDevice.GetVkDescriptorPool();
            initInfo.Allocator = VK_NULL_HANDLE;
            initInfo.MinImageCount = Graphics::NumConcurrentFrames;
            initInfo.ImageCount = Graphics::NumConcurrentFrames;
            initInfo.CheckVkResultFn = nullptr;
            initInfo.UseDynamicRendering = true;
            initInfo.PipelineRenderingCreateInfo = Graphics::VulkanUtils::InitVkPipelineRenderingCreateInfoKHR();
            initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
            initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &physicalDevice.GetVulkanContext().surfaceFormat.format;
            initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            initInfo.PipelineRenderingCreateInfo.stencilAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            context = new ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), Graphics::GraphicsBackendTypeToString(_graphicsBackend.GetType()), true, true, initInfo);
        }
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(context));

        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale(), 15);
        _imguiImpl->Stylize(dpiScale);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Finalize()
    {
        _imguiImpl.reset();

        _vertexBuffer.reset();
        _indexBuffer.reset();
        vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_SetMultisampling(UInt32 samples)
    {
        Events::QueueEvent(CreateUPtr<Events::MultisamplingChangeEvent>(samples));
    }
    //--------------------------------------------------------------------------

    bool MainFrameListener::_OnMultisamplingChangeEvent(Events::MultisamplingChangeEvent& evt)
    {
        Graphics::VulkanLogicalDevice& vulkanDevice = dynamic_cast<Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        auto& cswapchain = dynamic_cast<const Graphics::VulkanSwapchain&>(vulkanDevice.GetSwapchain());
        auto& swapchain = const_cast<Graphics::VulkanSwapchain&>(cswapchain);
        swapchain.SetMultisampling(VkSampleCountFlagBits(evt.msaaSamples));
        return true;
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {}
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        _RenderTriangle();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_RenderTriangle()
    {
        const Graphics::VulkanLogicalDevice& vulkanDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        auto pipelineOpt = vulkanDevice.GetGraphicsPipeline("VulkanTriangle"_sid);
        auto pipeline = pipelineOpt.value().get().GetVkPipeline();

        _commandBuffer = vulkanDevice.GetCurrentCommandBuffer().GetVkCommandBuffer();
        vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        VkDeviceSize offsets[1]{ 0 };
        VkBuffer vertexBuffer = _vertexBuffer->GetVkBuffer();
        VkBuffer indexBuffer = _indexBuffer->GetVkBuffer();

        // TODO: abstract away this
        auto& vulkanBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        auto instance = vulkanBackend.GetVkInstance();
        auto cmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)vkGetInstanceProcAddr(instance, "vkCmdSetRasterizationSamplesEXT");
        if (cmdSetRasterizationSamplesEXT)
        {
            const auto& swapchain = dynamic_cast<const Graphics::VulkanSwapchain&>(vulkanDevice.GetSwapchain());
            cmdSetRasterizationSamplesEXT(_commandBuffer, swapchain.GetMultisampling());
        }

        vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &vertexBuffer, offsets);
        vkCmdBindIndexBuffer(_commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(_commandBuffer, _indexCount, 1, 0, 0, 0);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_RenderImGui()
    {
        _imguiImpl->NewFrame();

        static constexpr auto applicationWindowFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("TriangleVulkan", nullptr, applicationWindowFlags);
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

        auto& vulkanLogicalDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        auto commandBuffer = vulkanLogicalDevice.GetCurrentCommandBuffer().GetVkCommandBuffer();
        auto* vulkanImGuiUtils = dynamic_cast<ImGuiUtils::ImGuiImplementationGlfwVulkan*>(_imguiImpl.get());
        vulkanImGuiUtils->SetCommandBuffer(commandBuffer);
        vulkanImGuiUtils->Render();

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------
}