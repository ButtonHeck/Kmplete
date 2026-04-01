#include "triangle_vulkan_main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/named_bool.h"
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
        , _uniformBuffers()
        , _indexCount(0)
        , _device(VK_NULL_HANDLE)
        , _commandBuffer(VK_NULL_HANDLE)
        , _imguiImpl(nullptr)
        , _assetsManager(assetsManager)
        , _multisamplingChangeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMultisamplingChangeEvent))
        , _shaderData(ShaderData{.colorMultiplier = 1.0f})
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
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const Graphics::VulkanRenderer& vulkanRenderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            {{ 0.95f,  0.95f}, {1.0f, 0.0f, 0.0f}},
            {{-0.95f,  0.97f}, {0.0f, 1.0f, 0.0f}},
            {{-0.92f, -0.95f}, {0.0f, 0.0f, 1.0f}}
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<UInt32> indices{ 0, 1, 2 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vertexBufferSize + indexBufferSize });

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
        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize }));
        _vertexBuffer->AddLayout(vertexBufferLayout);

        _indexBuffer.reset(vulkanBufferCreator.CreateIndexBufferPtr({ VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize }));

        {
            const auto copyCmd = vulkanRenderer.CreateCommandBuffer();
            copyCmd.Begin();
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBuffer.get(), { VkBufferCopy{.size = vertexBufferSize} });
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_indexBuffer.get(), { VkBufferCopy{.srcOffset = vertexBufferSize, .size = indexBufferSize}});
            copyCmd.End();
            vulkanDevice.GetGraphicsQueue().SyncSubmit(copyCmd);
        }

        const auto shaderUniformVariableBinding = 3;
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = shaderUniformVariableBinding;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        vulkanDevice.AddDescriptorSetLayout("TriangleVulkan_0"_sid, {});
        vulkanDevice.AddDescriptorSetLayout("TriangleVulkan_1"_sid, { layoutBinding });

        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            _uniformBuffers.emplace_back(vulkanBufferCreator.CreateUniformBufferPtr(
                { 0, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(ShaderData) },
                { vulkanDevice.GetDescriptorSetLayout("TriangleVulkan_1"_sid) },
                shaderUniformVariableBinding));
            _uniformBuffers[i]->Map();
        }

        const auto vertexShader = vulkanDevice.CreateShader(String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.vert.spv"));
        const auto fragmentShader = vulkanDevice.CreateShader(String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.frag.spv"));
        auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShader.GetShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "main"),
            fragmentShader.GetShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "main")
        };

        auto& pipeline = vulkanDevice.AddGraphicsPipeline("VulkanTriangle"_sid);
        pipeline.SetInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, "primitive restart"_false);
        pipeline.SetPolygonMode(VK_POLYGON_MODE_FILL);
        pipeline.SetCulling(VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        pipeline.SetDepthClamping(false);
        pipeline.SetRasterizerDiscard(false);
        pipeline.SetDepthBiasParameters("bias enabled"_false, 0.0f, 0.0f, 0.0f);
        pipeline.SetDepthTest(true);
        pipeline.SetDepthWrite(true);
        pipeline.SetDepthComparison(VK_COMPARE_OP_LESS_OR_EQUAL);
        pipeline.SetDepthBoundsTest(false);
        pipeline.SetStencilTest(false);
        pipeline.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipeline.SetStencilStates(Graphics::VulkanPresets::StencilOpState_Disabled, Graphics::VulkanPresets::StencilOpState_Disabled);
        pipeline.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VulkanPresets::ColorBlendAttachmentState_NoBlend);
        pipeline.AddDescriptorSetLayout(vulkanDevice.GetDescriptorSetLayout("TriangleVulkan_0"_sid));
        pipeline.AddDescriptorSetLayout(vulkanDevice.GetDescriptorSetLayout("TriangleVulkan_1"_sid));
        pipeline.AddVertexBufferAttributesBindings(*_vertexBuffer, 0);
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
            context = new ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), Graphics::GraphicsBackendTypeToString(_graphicsBackend.GetType()), "docking"_true, "viewports"_true, initInfo);
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

        _uniformBuffers.clear();
        _vertexBuffer.reset();
        _indexBuffer.reset();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_SetMultisampling(UInt32 samples)
    {
        Events::QueueEvent(CreateUPtr<Events::MultisamplingChangeEvent>(samples));
    }
    //--------------------------------------------------------------------------

    bool MainFrameListener::_OnMultisamplingChangeEvent(Events::MultisamplingChangeEvent& evt)
    {
        _graphicsBackend.SetMultisampling(evt.msaaSamples);
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
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const Graphics::VulkanLogicalDevice& vulkanDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        const Graphics::VulkanRenderer& vulkanRenderer = vulkanDevice.GetRenderer();
        const auto currentBufferIndex = vulkanGraphicsBackend.GetCurrentBufferIndex();

        const auto descriptorSetIndex = 1; // should match with triangle.frag "layout (set = 1, binding = ...)"
        _uniformBuffers[currentBufferIndex]->CopyToMappedMemory(0, &_shaderData, sizeof(ShaderData));

        vulkanRenderer.BeginRendering("VulkanTriangle"_sid, { VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() });
        vulkanRenderer.BindDescriptorSets("VulkanTriangle"_sid, descriptorSetIndex, { _uniformBuffers[currentBufferIndex]->GetVkDescriptorSet() });
        vulkanRenderer.BindGraphicsPipeline("VulkanTriangle"_sid);
        vulkanRenderer.BindVertexBuffers(0, { _vertexBuffer->GetVkBuffer() }, { VkDeviceSize{0}} );
        vulkanRenderer.BindIndexBuffer(_indexBuffer->GetVkBuffer());
        vulkanRenderer.DrawIndexed(_indexCount, 1, 0, 0, 0);
        vulkanRenderer.EndRendering();
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
        ImGui::SliderFloat("Color multiplier", &_shaderData.colorMultiplier, 0.2f, 1.0f);

        ImGui::End();

        auto& vulkanLogicalDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        auto commandBuffer = vulkanLogicalDevice.GetRenderer().GetCurrentCommandBuffer();
        auto* vulkanImGuiUtils = dynamic_cast<ImGuiUtils::ImGuiImplementationGlfwVulkan*>(_imguiImpl.get());
        vulkanImGuiUtils->SetCommandBuffer(commandBuffer);
        vulkanImGuiUtils->Render();

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------
}