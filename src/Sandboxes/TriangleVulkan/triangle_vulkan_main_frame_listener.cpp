#include "triangle_vulkan_main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
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
            float position[3];
            float color[4];
        };
        //--------------------------------------------------------------------------
    }


#define TRIANGLE_VULKAN_DYNAMIC_RENDERING true
#define USE_ORTHOGRAPHIC_CAMERA false


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, Input::InputManager* inputManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _inputManager(inputManager)
        , _vertexBuffer(nullptr)
        , _indexBuffer(nullptr)
        , _uniformBuffers()
        , _matrixUniformBuffers()
        , _indexCount(0)
        , _device(VK_NULL_HANDLE)
        , _commandBuffer(VK_NULL_HANDLE)
        , _imguiImpl(nullptr)
        , _assetsManager(assetsManager)
        , _multisamplingChangeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMultisamplingChangeEvent))
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnWindowResizeEvent))
        , _mouseButtonPressedHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMouseButtonPressedEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(MainFrameListener::_OnMouseScrollEvent))
        , _matrixShaderData()
        , _shaderData(ShaderData{.colorMultiplier = 1.0f})
#if USE_ORTHOGRAPHIC_CAMERA
        , _camera({0.0f, 0.0f, -2.0f }, Graphics::Camera::Type::FirstPerson)
#else
        , _camera({0.0f, 0.0f, -2.0f}, Graphics::Camera::Type::FirstPerson, 75.0f)
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

        _inputManager->MapInputToCallback({ Input::Code::Key_W, Input::NoCondition }, "move_up"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveUp, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_S, Input::NoCondition }, "move_down"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveDown, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_A, Input::NoCondition }, "move_left"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveLeft, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_D, Input::NoCondition }, "move_right"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveRight, std::get<int>(value) != 0);
            return true;
        });
#else
        _inputManager->MapInputToCallback({ Input::Code::Key_W, Input::NoCondition }, "move_forward"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveForward, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_S, Input::NoCondition }, "move_backward"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveBackward, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_A, Input::NoCondition }, "move_left"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveLeft, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_D, Input::NoCondition }, "move_right"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveRight, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_Space, Input::NoCondition }, "move_up"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveUp, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Key_LeftShift, Input::NoCondition }, "move_down"_sid, [this](Input::InputControlValue value) {
            _camera.Move(Graphics::Camera::MoveDown, std::get<int>(value) != 0);
            return true;
        });
        _inputManager->MapInputToCallback({ Input::Code::Mouse_Move, Input::NoCondition }, "rotate"_sid, [this](Input::InputControlValue value) {
            if (_mainWindow.GetCursorMode() == Window::CursorMode::Default)
            {
                return true;
            }

            const auto rotationValue = std::get<Math::Point2I>(value);
            _camera.Rotate(Math::Vec3F(-rotationValue.y * _camera.GetRotationSpeed(), rotationValue.x * _camera.GetRotationSpeed(), 0.0f));
            return true;
        });
#endif

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
        Graphics::VulkanDescriptorSetManager& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        const Vector<Vertex> vertices{
            // main RGB triangle
            {{-0.95f, -0.97f, 0.2f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{ 0.95f, -0.95f, 0.2f}, {0.0f, 1.0f, 0.0f, 1.0f}},
            {{-0.92f,  0.95f, 0.2f}, {0.0f, 0.0f, 1.0f, 1.0f}}
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<Vertex> vertices2{
            // grey-to-white triangle above main RGB triangle
            {{-0.50f,  0.90f, 0.1f}, {0.3f, 0.3f, 0.3f, 1.0f}},
            {{-0.75f,  0.25f, 0.1f}, {0.6f, 0.6f, 0.6f, 1.0f}},
            {{-0.25f,  0.25f, 0.1f}, {1.0f, 1.0f, 1.0f, 1.0f}},

            // reddish triangle below main RGB triangle (depth is outside of range, but it will be clamped due to DepthClamping option)
            {{-0.00f,  0.40f, 0.8f}, {1.0f, 0.3f, 0.3f, 1.0f}},
            {{-0.25f, -0.25f, 0.8f}, {1.0f, 0.6f, 0.6f, 1.0f}},
            {{ 0.25f, -0.25f, 0.8f}, {1.0f, 0.9f, 0.9f, 1.0f}},

            // half-transparent quad above everything
            {{-0.80f, -0.80f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.25f}},
            {{ 0.80f, -0.80f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.25f}},
            {{-0.80f,  0.80f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.25f}},
            {{-0.80f,  0.80f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.25f}},
            {{ 0.80f, -0.80f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.25f}},
            {{ 0.80f,  0.80f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.25f}},
        };
        const auto vertex2BufferSize = UInt32(vertices2.size() * sizeof(Vertex));

        const Vector<UInt32> indices{ 0, 1, 2 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vertexBufferSize + vertex2BufferSize + indexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize + indexBufferSize, (char*)vertices2.data(), vertex2BufferSize);
        stagingBuffer.Unmap("flush"_true);

        const auto vertexBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{Graphics::ShaderDataType::Float3, 0},
            Graphics::BufferElement{Graphics::ShaderDataType::Float4, 1}
        });
        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize + vertex2BufferSize }));
        _vertexBuffer->AddLayout(vertexBufferLayout);

        _indexBuffer.reset(vulkanBufferCreator.CreateIndexBufferPtr({ VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize }));

        {
            const auto copyCmd = vulkanRenderer.CreateCommandBuffer();
            copyCmd.Begin();
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBuffer.get(), 0, 0, vertexBufferSize);
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_indexBuffer.get(), vertexBufferSize, 0, indexBufferSize);
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBuffer.get(), vertexBufferSize + indexBufferSize, vertexBufferSize, vertex2BufferSize);
            copyCmd.End();
            vulkanDevice.GetGraphicsQueue().SyncSubmit(copyCmd);
        }

        const auto matricesUniformBindingNumber = 0;
        VkDescriptorSetLayoutBinding matricesLayoutBinding{};
        matricesLayoutBinding.binding = matricesUniformBindingNumber;
        matricesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        matricesLayoutBinding.descriptorCount = 1;
        matricesLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        const auto matricesLayout = descriptorSetManager.AddDescriptorSetLayout("TriangleVulkan_DS_Matrix"_sid, {matricesLayoutBinding});
        descriptorSetManager.AllocateDescriptorSets(matricesLayout, "Matrices_Set"_sid, 1, "per frame"_true);

        const auto colorMultiplierUniformBindingNumber = 3;
        VkDescriptorSetLayoutBinding colorMultiplierLayoutBinding{};
        colorMultiplierLayoutBinding.binding = colorMultiplierUniformBindingNumber;
        colorMultiplierLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        colorMultiplierLayoutBinding.descriptorCount = 1;
        colorMultiplierLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        const auto colorMultiplierLayout = descriptorSetManager.AddDescriptorSetLayout("TriangleVulkan_DS_ColorMultiplier"_sid, { colorMultiplierLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(colorMultiplierLayout, "ColorMultipler_Set"_sid, 1, "per frame"_true);

        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            _uniformBuffers.emplace_back(vulkanBufferCreator.CreateUniformBufferPtr({ 0, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(ShaderData) }));
            _uniformBuffers[i]->Map();
            descriptorSetManager.SetUniformBufferDescriptor("ColorMultipler_Set"_sid, 0, "per frame"_true, i, *_uniformBuffers[i].get(), colorMultiplierUniformBindingNumber);

            _matrixUniformBuffers.emplace_back(vulkanBufferCreator.CreateUniformBufferPtr({ 0, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(MatrixShaderData) }));
            _matrixUniformBuffers[i]->Map();
            descriptorSetManager.SetUniformBufferDescriptor("Matrices_Set"_sid, 0, "per frame"_true, i, *_matrixUniformBuffers[i].get(), matricesUniformBindingNumber);
        }

        auto& pipeline = vulkanDevice.AddGraphicsPipeline("VulkanTriangle_Pipeline"_sid);
        pipeline.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipeline.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VulkanPresets::ColorBlendAttachmentState_AlphaBlending);
        pipeline.AddDescriptorSetLayout(descriptorSetManager.GetDescriptorSetLayout("TriangleVulkan_DS_Matrix"_sid));
        pipeline.AddDescriptorSetLayout(descriptorSetManager.GetDescriptorSetLayout("TriangleVulkan_DS_ColorMultiplier"_sid));

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.frag.spv");

        const auto vertexShaderModule = vulkanDevice.CreateShaderModule(vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.CreateShaderModule(fragmentShaderPath);
        auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.GetShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "main"),
            fragmentShaderModule.GetShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "main")
        };
        pipeline.AddShaderStages(std::move(shaderStages));

#if !TRIANGLE_VULKAN_DYNAMIC_RENDERING
        pipeline.SetInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, "primitive restart"_false);
        pipeline.SetPolygonMode(VK_POLYGON_MODE_FILL);
        pipeline.SetCulling(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        pipeline.SetDepthClamping(true);
        pipeline.SetRasterizerDiscard(false);
        pipeline.SetDepthBiasParameters("bias enabled"_false, 0.0f, 0.0f, 0.0f);
        pipeline.SetDepthTest(true);
        pipeline.SetDepthWrite(true);
        pipeline.SetDepthComparison(VK_COMPARE_OP_LESS_OR_EQUAL);
        pipeline.SetDepthBoundsTest(false);
        pipeline.SetStencilTest(true);
        pipeline.SetStencilStates(Graphics::VulkanPresets::StencilOpState_Disabled, Graphics::VulkanPresets::StencilOpState_Disabled);
        pipeline.AddVertexBufferAttributesBindings(*_vertexBuffer, 0);
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_SCISSOR);
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT);
#else
        // additional dynamic states for testing
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE);           //renderer.SetDepthTestEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE);          //renderer.SetDepthWriteEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_COMPARE_OP);            //renderer.SetDepthCompareOp(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE);    //renderer.SetDepthBoundsEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_BOUNDS);                //renderer.SetDepthBounds(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE);           //renderer.SetDepthBiasEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_BIAS);                  //renderer.SetDepthBias(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT);      //renderer.SetDepthClampEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_CLAMP_RANGE_EXT);       //renderer.SetDepthClampRange(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT);       //renderer.SetDepthClipEnabled(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE);     //renderer.SetStencilTestEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_STENCIL_OP);              //renderer.SetStencilOp(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK);    //renderer.SetStencilCompareMask(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_STENCIL_WRITE_MASK);      //renderer.SetStencilWriteMask(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_STENCIL_REFERENCE);       //renderer.SetStencilReference(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT); //renderer.SetViewportWithCount(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT);  //renderer.SetScissorWithCount(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_LINE_WIDTH);                  //renderer.SetLineWidth(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT);     //renderer.SetLineStippleEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_LINE_STIPPLE);                //renderer.SetLineStipple(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT); //renderer.SetLineRasterizationMode(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT);      //renderer.SetColorWriteEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_COLOR_WRITE_MASK_EXT);        //renderer.SetColorWriteMask(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT);      //renderer.SetColorBlendEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT);    //renderer.SetColorBlendEquation(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY);          //renderer.SetPrimitiveTopology(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE);    //renderer.SetPrimitiveRestartEnabled(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT);    //renderer.SetDiscardRectangleEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT);           //renderer.SetDiscardRectangle(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_DISCARD_RECTANGLE_MODE_EXT);      //renderer.SetDiscardRectangleMode(...)

        //pipeline.AddDynamicState(VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT); //renderer.SetSampleLocationsEnabled(...)
        //pipeline.AddDynamicState(VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT);      //renderer.SetSampleLocations(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_SAMPLE_MASK_EXT);           //renderer.SetSampleMask(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT);    //renderer.SetAlphaToCoverageEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT);         //renderer.SetAlphaToOneEnabled(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT); //renderer.SetLogicOpEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_LOGIC_OP_EXT);        //renderer.SetLogicOp(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_VERTEX_INPUT_EXT);            //renderer.SetVertexInput(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE); //renderer.BindVertexBuffers2(...)

        pipeline.AddDynamicState(VK_DYNAMIC_STATE_CULL_MODE);                   //renderer.SetCullMode(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_FRONT_FACE);                  //renderer.SetFrontFace(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_BLEND_CONSTANTS);             //renderer.SetBlendConstants(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE);   //renderer.SetRasterizerDiscardEnabled(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT);   //renderer.SetRasterizationSamples(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR);   //renderer.SetFragmentShadingRate(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_POLYGON_MODE_EXT);            //renderer.SetPolygonMode(...)
        pipeline.AddDynamicState(VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT);   //renderer.SetProvokingVertexMode(...)

        const Vector<StringID> descriptorSetsLayoutsSids = { "TriangleVulkan_DS_Matrix"_sid, "TriangleVulkan_DS_ColorMultiplier"_sid };
        vulkanDevice.AddShaderObject("TriangleVulkan_vertex"_sid, vertexShaderPath, VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT, "linked"_true, descriptorSetsLayoutsSids);
        vulkanDevice.AddShaderObject("TriangleVulkan_fragment"_sid, fragmentShaderPath, VK_SHADER_STAGE_FRAGMENT_BIT, 0, "linked"_true, descriptorSetsLayoutsSids);
#endif

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
            initInfo.DescriptorPool = logicalDevice.GetDescriptorSetManager().GetVkDescriptorPool();
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

        _matrixUniformBuffers.clear();
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
        _RenderTriangle();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_RenderTriangle()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const Graphics::VulkanLogicalDevice& vulkanDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        const Graphics::VulkanRenderer& vulkanRenderer = vulkanDevice.GetRenderer();
        const Graphics::VulkanDescriptorSetManager& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        _matrixShaderData.viewMatrix = _camera.GetViewMatrix();
        _matrixShaderData.projectionMatrix = _camera.GetProjectionMatrix();
        _matrixShaderData.modelMatrix = Math::Mat4(1.0f);

        const auto currentBufferIndex = vulkanGraphicsBackend.GetCurrentBufferIndex();
        _uniformBuffers[currentBufferIndex]->CopyToMappedMemory(0, &_shaderData, sizeof(ShaderData));
        _matrixUniformBuffers[currentBufferIndex]->CopyToMappedMemory(0, &_matrixShaderData, sizeof(MatrixShaderData));

        vulkanRenderer.BeginRendering("VulkanTriangle_Pipeline"_sid, { VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() });
        vulkanRenderer.BindDescriptorSets("VulkanTriangle_Pipeline"_sid, 0, { 
            descriptorSetManager.GetDescriptorSet("Matrices_Set"_sid, 0, "per frame"_true), 
            descriptorSetManager.GetDescriptorSet("ColorMultipler_Set"_sid, 0, "per frame"_true)
        });
        vulkanRenderer.BindGraphicsPipeline("VulkanTriangle_Pipeline"_sid);
        vulkanRenderer.BindIndexBuffer(_indexBuffer->GetVkBuffer());
        vulkanRenderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());

#if !TRIANGLE_VULKAN_DYNAMIC_RENDERING
        vulkanRenderer.BindVertexBuffers(0, { _vertexBuffer->GetVkBuffer() }, { VkDeviceSize{0}} );
        vulkanRenderer.SetViewport(VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f });
        vulkanRenderer.SetScissor(VkRect2D{ .offset = VkOffset2D{.x = 0, .y = 0 }, .extent = vulkanDevice.GetCurrentExtent() });
#else
        // dynamic rendering functions tests
        vulkanRenderer.SetDepthTestEnabled(true);
        vulkanRenderer.SetDepthWriteEnabled(true);
        vulkanRenderer.SetDepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL);
        vulkanRenderer.SetDepthBoundsEnabled(false);
        vulkanRenderer.SetDepthBounds(0.0f, 1.0f);
        vulkanRenderer.SetDepthBiasEnabled(true);
        vulkanRenderer.SetDepthBias(0.0f, 0.0f, 0.0f);
        vulkanRenderer.SetDepthClampEnabled(true);
        vulkanRenderer.SetDepthClampRange(VK_DEPTH_CLAMP_MODE_VIEWPORT_RANGE_EXT, 0.0f, 1.0f);
        vulkanRenderer.SetDepthClipEnabled(true);

        vulkanRenderer.SetStencilTestEnabled(false);
        vulkanRenderer.SetStencilOp(VK_STENCIL_FACE_FRONT_BIT, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS);
        vulkanRenderer.SetStencilCompareMask(VK_STENCIL_FACE_FRONT_BIT, 0);
        vulkanRenderer.SetStencilWriteMask(VK_STENCIL_FACE_FRONT_BIT, 0);
        vulkanRenderer.SetStencilReference(VK_STENCIL_FACE_FRONT_BIT, 0);

        vulkanRenderer.SetViewportWithCount({ VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f}});
        vulkanRenderer.SetScissorWithCount({ VkRect2D{ .offset = VkOffset2D{.x = 0, .y = 0 }, .extent = vulkanDevice.GetCurrentExtent() }});

        vulkanRenderer.SetLineWidth(1.0f);
        vulkanRenderer.SetLineStippleEnabled(false);
        vulkanRenderer.SetLineStipple(1, 1);
        vulkanRenderer.SetLineRasterizationMode(VK_LINE_RASTERIZATION_MODE_DEFAULT);

        vulkanRenderer.SetColorWriteEnabled(1, { VK_TRUE });
        vulkanRenderer.SetColorWriteMask(0, 1, { VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT });
        vulkanRenderer.SetColorBlendEnabled(0, 1, { VK_TRUE });
        vulkanRenderer.SetColorBlendEquation(0, 1, { Graphics::VulkanPresets::ColorBlendEquation_AlphaBlending });

        vulkanRenderer.SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
        vulkanRenderer.SetPrimitiveRestartEnabled(false);

        vulkanRenderer.SetDiscardRectangleEnabled(false);
        vulkanRenderer.SetDiscardRectangle(0, 8,
            { VkRect2D{.offset = VkOffset2D{.x = 1000, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1030, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1060, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1090, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1120, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1150, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1180, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}},
             VkRect2D{.offset = VkOffset2D{.x = 1210, .y = 1000}, .extent = VkExtent2D{.width = 25, .height = 25}} });
        vulkanRenderer.SetDiscardRectangleMode(VK_DISCARD_RECTANGLE_MODE_EXCLUSIVE_EXT);

        vulkanRenderer.SetAlphaToCoverageEnabled(false);
        vulkanRenderer.SetAlphaToOneEnabled(false);

        vulkanRenderer.SetLogicOpEnabled(false);
        vulkanRenderer.SetLogicOp(VK_LOGIC_OP_COPY);

        const auto& [inputBindingsDescriptions, attributeDescriptions] = _vertexBuffer->GetDynamicBindingsDescriptions(0);
        vulkanRenderer.SetVertexInput(inputBindingsDescriptions, attributeDescriptions);

        vulkanRenderer.SetCullMode(VK_CULL_MODE_NONE);
        vulkanRenderer.SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
        vulkanRenderer.SetBlendConstants({1, 1, 1, 1});
        vulkanRenderer.SetRasterizerDiscardEnabled(false);
        vulkanRenderer.SetSampleLocationsEnabled(false);
        vulkanRenderer.SetPolygonMode(VK_POLYGON_MODE_FILL);
        vulkanRenderer.SetProvokingVertexMode(VK_PROVOKING_VERTEX_MODE_FIRST_VERTEX_EXT);
        vulkanRenderer.SetSampleMask(vulkanDevice.GetMultisampling(), {0xFF});
        vulkanRenderer.BindVertexBuffers2(0, { _vertexBuffer->GetVkBuffer() }, { VkDeviceSize{0} }, { VkDeviceSize{420} }, { VkDeviceSize{sizeof(Vertex)} });
        vulkanRenderer.BindShaderObjects(
            { VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT },
            { "TriangleVulkan_vertex"_sid, "TriangleVulkan_fragment"_sid }
        );
#endif

        // drawing
        vulkanRenderer.DrawIndexed(_indexCount, 1, 0, 0, 0);
        vulkanRenderer.Draw(12, 1, 3, 0);
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