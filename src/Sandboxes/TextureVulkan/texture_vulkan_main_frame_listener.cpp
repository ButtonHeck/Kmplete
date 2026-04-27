#include "texture_vulkan_main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/ImGui/implementation_glfw_vulkan.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto MatricesAndTextureDSLayout_SID = "TextureVulkan_DS"_sid;
    static constexpr auto MatricesAndTextureDS_SID = "MatrixAndTexture_Set"_sid;
    static constexpr auto SamplerDSLayout_SID = "TextureVulkan_DS_sampler"_sid;
    static constexpr auto SamplerDS_SID = "Sampler_Set"_sid;

    static constexpr auto Pipeline_SID = "TextureVulkan_Pipeline"_sid;

    static constexpr auto VertexShader_SID = "TextureVulkan_vertex"_sid;
    static constexpr auto FragmentShader_SID = "TextureVulkan_fragment"_sid;

    static constexpr auto TextureMetal_SID = "texture_metal"_sid;
    static constexpr auto TextureMarble_SID = "texture_marble"_sid;
    static constexpr auto TextureExample_SID = "texture_example"_sid;

    static constexpr auto MatricesBindingIndex = 0;
    static constexpr auto TextureBindingIndex = 1;
    static constexpr auto SamplerBindingIndex = 0;


    namespace
    {
        struct Vertex
        {
            float position[3];
            float uv[2];
        };
        //--------------------------------------------------------------------------
    }

    using namespace Graphics::VKBits;


#define USE_ORTHOGRAPHIC_CAMERA false


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, Input::InputManager* inputManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _inputManager(inputManager)
        , _vertexBuffer(nullptr)
        , _indexBuffer(nullptr)
        , _uniformBuffers()
        , _indexCount(0)
        , _device(VK_NULL_HANDLE)
        , _imguiImpl(nullptr)
        , _assetsManager(assetsManager)
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnWindowResizeEvent))
        , _mouseButtonPressedHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMouseButtonPressedEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMouseScrollEvent))
        , _matrixShaderData()
#if USE_ORTHOGRAPHIC_CAMERA
        , _camera({ 0.0f, 0.0f, -2.0f }, Graphics::Camera::Type::FirstPerson)
#else
        , _camera({ 0.0f, 0.0f, -2.0f }, Graphics::Camera::Type::FirstPerson, 75.0f)
#endif
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
        _camera.SetMovementSpeed(0.0025f);
        _camera.SetRotationSpeed(0.1f);
        _camera.SetAspectRatio(float(_mainWindow.GetSize().x) / float(_mainWindow.GetSize().y));
        _camera.SetZNear(0.1f);
        _camera.SetZFar(10.0f);
#if USE_ORTHOGRAPHIC_CAMERA
        _camera.SetScale(4.0f);
        _camera.SetOrthographicParameters(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);
        _camera.SetApplyAspectRatioFix(true);

        _inputManager->MapInputToCallback(Input::Code::Key_W, "move_up"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveUp, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_S, "move_down"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveDown, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_A, "move_left"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveLeft, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_D, "move_right"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveRight, std::get<int>(value) != 0);
            return true;
        });
#else
        _inputManager->MapInputToCallback(Input::Code::Key_W, "move_forward"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveForward, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_S, "move_backward"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveBackward, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_A, "move_left"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveLeft, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_D, "move_right"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveRight, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_Space, "move_up"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveUp, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Key_LeftShift, "move_down"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveDown, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback(Input::Code::Mouse_Move, "rotate"_sid, [this](Input::InputControlValue value) {
            if (_mainWindow.GetCursorMode() == Window::CursorMode::Default)
            {
                return true;
            }

            const auto rotationValue = std::get<Math::Point2I>(value);
            _camera.Rotate(Math::Vec3F(-rotationValue.y * _camera.GetRotationSpeed(), rotationValue.x * _camera.GetRotationSpeed(), 0.0f));
            return true;
        });
#endif

        _InitializeTextureQuad();
        _InitializeImGui(_mainWindow.GetDPIScale());
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_InitializeTextureQuad()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        const auto& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();
        _device = vulkanDevice.GetVkDevice();
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const auto& vulkanRenderer = vulkanDevice.GetRenderer();
        auto& textureAssetManager = _assetsManager.GetTextureAssetManager();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& samplersStorage = vulkanDevice.GetSamplersStorage();

        const Vector<Vertex> vertices{
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<UInt32> indices{ 0, 1, 2, 2, 3, 0 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + indexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        const auto vertexBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float3, 0 },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, 1 }
        });
        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize }));
        _vertexBuffer->AddLayout(vertexBufferLayout);

        _indexBuffer.reset(vulkanBufferCreator.CreateIndexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, indexBufferSize }));

        {
            const auto copyCmd = vulkanRenderer.CreateCommandBuffer();
            copyCmd.Begin();
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBuffer.get(), 0, 0, vertexBufferSize);
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_indexBuffer.get(), vertexBufferSize, 0, indexBufferSize);
            copyCmd.End();
            vulkanDevice.GetGraphicsQueue().SyncSubmit(copyCmd);
        }

        VkDescriptorSetLayoutBinding matricesLayoutBinding{ MatricesBindingIndex, VK_DescriptorType_UniformBuffer, 1, VK_ShaderStage_Vertex };
        VkDescriptorSetLayoutBinding textureLayoutBinding{ TextureBindingIndex, VK_DescriptorType_SampledImage, 1, VK_ShaderStage_Fragment };
        const auto matrixAndTextureLayout = descriptorSetManager.AddDescriptorSetLayout(MatricesAndTextureDSLayout_SID, { matricesLayoutBinding, textureLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(matrixAndTextureLayout, MatricesAndTextureDS_SID, 1, "per frame"_true);

        VkDescriptorSetLayoutBinding samplerLayoutBinding{ SamplerBindingIndex, VK_DescriptorType_Sampler, 1, VK_ShaderStage_Fragment };
        const auto samplerLayout = descriptorSetManager.AddDescriptorSetLayout(SamplerDSLayout_SID, { samplerLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(samplerLayout, SamplerDS_SID, 1, "per frame"_true);

        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            _uniformBuffers.emplace_back(vulkanBufferCreator.CreateUniformBufferPtr({ 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, sizeof(MatrixShaderData) }));
            _uniformBuffers[i]->Map();

            descriptorSetManager.SetUniformBufferDescriptor(MatricesAndTextureDS_SID, 0, "per frame"_true, i, *_uniformBuffers[i].get(), MatricesBindingIndex);
            descriptorSetManager.SetSampledImageDescriptor(
                MatricesAndTextureDS_SID, 0, "per frame"_true, i,
                dynamic_cast<Graphics::VulkanTexture&>(textureAssetManager.GetAsset(TextureMetal_SID).GetTexture()).GetVkImageView(), TextureBindingIndex
            );
            descriptorSetManager.SetSamplerDescriptor(SamplerDS_SID, 0, "per frame"_true, i, samplersStorage.GetSampler(Graphics::SamplerDefaultNearestSid), SamplerBindingIndex);
        }

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_AlphaBlending);
        pipelineParams.AddDescriptorSetLayout(matrixAndTextureLayout);
        pipelineParams.AddDescriptorSetLayout(samplerLayout);

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("texture.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("texture.frag.spv");

        const auto vertexShaderModule = vulkanDevice.CreateShaderModule(vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.CreateShaderModule(fragmentShaderPath);
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };
        pipelineParams.AddShaderStages(shaderStages);

        pipelineParams.AddDynamicState(VK_Dynamic_DepthTestEnable);       //renderer.SetDepthTestEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthWriteEnable);      //renderer.SetDepthWriteEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthCompareOp);        //renderer.SetDepthCompareOp(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthBoundsTestEnable); //renderer.SetDepthBoundsEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthBounds);           //renderer.SetDepthBounds(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthBiasEnable);       //renderer.SetDepthBiasEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthBias);             //renderer.SetDepthBias(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthClampEnable);      //renderer.SetDepthClampEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthClampRange);       //renderer.SetDepthClampRange(...)
        pipelineParams.AddDynamicState(VK_Dynamic_DepthClipEnable);       //renderer.SetDepthClipEnabled(...)

        pipelineParams.AddDynamicState(VK_Dynamic_StencilTestEnable);     //renderer.SetStencilTestEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_StencilOp);             //renderer.SetStencilOp(...)
        pipelineParams.AddDynamicState(VK_Dynamic_StencilCompareMask);    //renderer.SetStencilCompareMask(...)
        pipelineParams.AddDynamicState(VK_Dynamic_StencilWriteMask);      //renderer.SetStencilWriteMask(...)
        pipelineParams.AddDynamicState(VK_Dynamic_StencilReference);      //renderer.SetStencilReference(...)

        pipelineParams.AddDynamicState(VK_Dynamic_ViewportWithCount); //renderer.SetViewportWithCount(...)
        pipelineParams.AddDynamicState(VK_Dynamic_ScissorWithCount);  //renderer.SetScissorWithCount(...)

        pipelineParams.AddDynamicState(VK_Dynamic_ColorWriteEnable);      //renderer.SetColorWriteEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_ColorWriteMask);        //renderer.SetColorWriteMask(...)
        pipelineParams.AddDynamicState(VK_Dynamic_ColorBlendEnable);      //renderer.SetColorBlendEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_ColorBlendEquation);    //renderer.SetColorBlendEquation(...)

        pipelineParams.AddDynamicState(VK_Dynamic_PrimitiveTopology);         //renderer.SetPrimitiveTopology(...)
        pipelineParams.AddDynamicState(VK_Dynamic_PrimitiveRestartEnable);    //renderer.SetPrimitiveRestartEnabled(...)

        pipelineParams.AddDynamicState(VK_Dynamic_DiscardRectangleEnable);    //renderer.SetDiscardRectangleEnabled(...)

        pipelineParams.AddDynamicState(VK_Dynamic_SampleLocationsEnable); //renderer.SetSampleLocationsEnabled(...)

        pipelineParams.AddDynamicState(VK_Dynamic_SampleMask);           //renderer.SetSampleMask(...)

        pipelineParams.AddDynamicState(VK_Dynamic_AlphaToCoverageEnable);    //renderer.SetAlphaToCoverageEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_AlphaToOneEnable);         //renderer.SetAlphaToOneEnabled(...)

        pipelineParams.AddDynamicState(VK_Dynamic_LogicOpEnable); //renderer.SetLogicOpEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_LogicOp);       //renderer.SetLogicOp(...)

        pipelineParams.AddDynamicState(VK_Dynamic_VertexInput);               //renderer.SetVertexInput(...)
        pipelineParams.AddDynamicState(VK_Dynamic_VertexInputBindingStride);  //renderer.BindVertexBuffers2(...)

        pipelineParams.AddDynamicState(VK_Dynamic_CullMode);                  //renderer.SetCullMode(...)
        pipelineParams.AddDynamicState(VK_Dynamic_FrontFace);                 //renderer.SetFrontFace(...)
        pipelineParams.AddDynamicState(VK_Dynamic_BlendConstants);            //renderer.SetBlendConstants(...)
        pipelineParams.AddDynamicState(VK_Dynamic_RasterizerDiscardEnable);   //renderer.SetRasterizerDiscardEnabled(...)
        pipelineParams.AddDynamicState(VK_Dynamic_RasterizationSamples);      //renderer.SetRasterizationSamples(...)
        pipelineParams.AddDynamicState(VK_Dynamic_FragmentShadingRate);       //renderer.SetFragmentShadingRate(...)
        pipelineParams.AddDynamicState(VK_Dynamic_PolygonMode);               //renderer.SetPolygonMode(...)

        const Vector<StringID> descriptorSetsLayoutsSids = { MatricesAndTextureDSLayout_SID, SamplerDSLayout_SID };
        vulkanDevice.AddShaderObject(VertexShader_SID, vertexShaderPath, VK_ShaderStage_Vertex, VK_ShaderStage_Fragment, "linked"_true, descriptorSetsLayoutsSids);
        vulkanDevice.AddShaderObject(FragmentShader_SID, fragmentShaderPath, VK_ShaderStage_Fragment, 0, "linked"_true, descriptorSetsLayoutsSids);

        vulkanDevice.AddGraphicsPipeline(Pipeline_SID, pipelineParams);
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
            initInfo.DescriptorPool = logicalDevice.GetDescriptorSetManager().GetVkDescriptorPool();
            initInfo.Allocator = VK_NULL_HANDLE;
            initInfo.MinImageCount = Graphics::NumConcurrentFrames;
            initInfo.ImageCount = Graphics::NumConcurrentFrames;
            initInfo.CheckVkResultFn = nullptr;
            initInfo.UseDynamicRendering = true;
            initInfo.PipelineRenderingCreateInfo = Graphics::VKUtils::InitVkPipelineRenderingCreateInfoKHR();
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

    bool MainFrameListener::_OnWindowResizeEvent(Events::WindowResizeEvent& evt)
    {
        if (evt.GetWidth() > 0 && evt.GetHeight())
        {
            _camera.SetAspectRatio(float(evt.GetWidth()) / float(evt.GetHeight()));
        }
        return true;
    }
    //--------------------------------------------------------------------------

    bool MainFrameListener::_OnMouseButtonPressedEvent(Events::MouseButtonPressEvent& evt)
    {
        if (evt.GetMouseButton() == Input::Code::Mouse_ButtonRight)
        {
            if (_mainWindow.GetCursorMode() == Window::CursorMode::Default)
            {
                _mainWindow.SetCursorMode(Window::CursorMode::Disabled);
            }
            else
            {
                _mainWindow.SetCursorMode(Window::CursorMode::Default);
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool MainFrameListener::_OnMouseScrollEvent(Events::MouseScrollEvent& evt)
    {
#if USE_ORTHOGRAPHIC_CAMERA
        _camera.SetScaleDelta(evt.GetYOffset());
#else
        _camera.SetFOVDelta(evt.GetYOffset());
#endif
        return true;
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Update(float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {
        _camera.Update(frameTimestep);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        _RenderTextureQuad();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_RenderTextureQuad()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        _matrixShaderData.viewMatrix = _camera.GetViewMatrix();
        _matrixShaderData.projectionMatrix = _camera.GetProjectionMatrix();
        _matrixShaderData.modelMatrix = Math::Mat4(1.0f);

        _uniformBuffers[vulkanGraphicsBackend.GetCurrentBufferIndex()]->CopyToMappedMemory(0, &_matrixShaderData, sizeof(MatrixShaderData));

        renderer.BeginRendering(Pipeline_SID, { VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() });
        renderer.BindDescriptorSets(Pipeline_SID, 0, {
            descriptorSetManager.GetDescriptorSet(MatricesAndTextureDS_SID, 0, "per frame"_true),
            descriptorSetManager.GetDescriptorSet(SamplerDS_SID, 0, "per frame"_true)
        });
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(0, { _vertexBuffer->GetVkBuffer() }, { 0 });
        renderer.BindIndexBuffer(_indexBuffer->GetVkBuffer());
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());

        renderer.SetDepthTestEnabled(true);
        renderer.SetDepthWriteEnabled(true);
        renderer.SetDepthCompareOp(VK_Compare_LessOrEqual);
        renderer.SetDepthBoundsEnabled(false);
        renderer.SetDepthBounds(0.0f, 1.0f);
        renderer.SetDepthBiasEnabled(true);
        renderer.SetDepthBias(0.0f, 0.0f, 0.0f);
        renderer.SetDepthClampEnabled(true);
        renderer.SetDepthClampRange(VK_DepthClamp_ViewportRange, 0.0f, 1.0f);
        renderer.SetDepthClipEnabled(true);

        renderer.SetStencilTestEnabled(false);
        renderer.SetStencilOp(VK_StencilFace_Front, VK_Stencil_Keep, VK_Stencil_Keep, VK_Stencil_Keep, VK_Compare_Always);
        renderer.SetStencilCompareMask(VK_StencilFace_Front, 0);
        renderer.SetStencilWriteMask(VK_StencilFace_Front, 0);
        renderer.SetStencilReference(VK_StencilFace_Front, 0);

        renderer.SetViewportWithCount({ VkViewport{.x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f} });
        renderer.SetScissorWithCount({ VkRect2D{.offset = VkOffset2D{.x = 0, .y = 0 }, .extent = vulkanDevice.GetCurrentExtent() } });

        renderer.SetColorWriteEnabled(1, { VK_TRUE });
        renderer.SetColorWriteMask(0, 1, { VK_Color_RGBA });
        renderer.SetColorBlendEnabled(0, 1, { VK_TRUE });
        renderer.SetColorBlendEquation(0, 1, { Graphics::VKPresets::ColorBlendEquation_AlphaBlending });

        renderer.SetPrimitiveTopology(VK_Primitive_TriangleList);
        renderer.SetPrimitiveRestartEnabled(false);

        renderer.SetDiscardRectangleEnabled(false);

        renderer.SetSampleLocationsEnabled(false);

        renderer.SetAlphaToCoverageEnabled(false);
        renderer.SetAlphaToOneEnabled(false);

        renderer.SetLogicOpEnabled(false);
        renderer.SetLogicOp(VK_LogicOp_Copy);

        const auto& [inputBindingsDescriptions, attributeDescriptions] = _vertexBuffer->GetDynamicBindingsDescriptions(0);
        renderer.SetVertexInput(inputBindingsDescriptions, attributeDescriptions);

        renderer.SetCullMode(VK_Cull_None);
        renderer.SetFrontFace(VK_FrontFace_CounterClockwise);
        renderer.SetBlendConstants({ 1, 1, 1, 1 });
        renderer.SetRasterizerDiscardEnabled(false);
        renderer.SetPolygonMode(VK_Polygon_Fill);
        renderer.SetSampleMask(vulkanDevice.GetMultisampling(), { 0xFF });
        renderer.BindShaderObjects(
            { VK_ShaderStage_Vertex, VK_ShaderStage_Fragment },
            { VertexShader_SID, FragmentShader_SID }
        );

        // drawing
        renderer.DrawIndexed(_indexCount, 1, 0, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_RenderImGui()
    {
        _imguiImpl->NewFrame();

        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        auto& textureAssetManager = _assetsManager.GetTextureAssetManager();
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& samplersStorage = vulkanDevice.GetSamplersStorage();

        static constexpr auto applicationWindowFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("TriangleVulkan", nullptr, applicationWindowFlags);
        if (ImGui::Button("Metal"))
        {
            descriptorSetManager.SetSampledImageDescriptor(
                MatricesAndTextureDS_SID, 0, "per frame"_true,
                dynamic_cast<Graphics::VulkanTexture&>(textureAssetManager.GetAsset(TextureMetal_SID).GetTexture()).GetVkImageView(), TextureBindingIndex
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("Marble"))
        {
            descriptorSetManager.SetSampledImageDescriptor(
                MatricesAndTextureDS_SID, 0, "per frame"_true,
                dynamic_cast<Graphics::VulkanTexture&>(textureAssetManager.GetAsset(TextureMarble_SID).GetTexture()).GetVkImageView(), TextureBindingIndex
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("Example"))
        {
            descriptorSetManager.SetSampledImageDescriptor(
                MatricesAndTextureDS_SID, 0, "per frame"_true,
                dynamic_cast<Graphics::VulkanTexture&>(textureAssetManager.GetAsset(TextureExample_SID).GetTexture()).GetVkImageView(), TextureBindingIndex
            );
        }
        ImGui::SliderFloat("LOD bias", &_matrixShaderData.lodBias, -8.0f, 8.0f);
        ImGui::SliderInt("Tiling", &_matrixShaderData.tiling, 1, 4);

        if (ImGui::Button("Nearest filter"))
        {
            descriptorSetManager.SetSamplerDescriptor(SamplerDS_SID, 0, "per frame"_true, samplersStorage.GetSampler(Graphics::SamplerDefaultNearestSid), SamplerBindingIndex);
        }
        ImGui::SameLine();
        if (ImGui::Button("Linear filter"))
        {
            descriptorSetManager.SetSamplerDescriptor(SamplerDS_SID, 0, "per frame"_true, samplersStorage.GetSampler(Graphics::SamplerDefaultLinearSid), SamplerBindingIndex);
        }

        ImGui::End();

        auto commandBuffer = vulkanDevice.GetRenderer().GetCurrentCommandBuffer();
        auto* vulkanImGuiUtils = dynamic_cast<ImGuiUtils::ImGuiImplementationGlfwVulkan*>(_imguiImpl.get());
        vulkanImGuiUtils->SetCommandBuffer(commandBuffer);
        vulkanImGuiUtils->Render();

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------
}