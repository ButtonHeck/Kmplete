#include "uniform_buffers_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/memory_utils.h"
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
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto MatricesDSLayout_SID = "MatricesDSLayout"_sid;
    static constexpr auto MatricesDS_SID = "Matrices_Set"_sid;

    static constexpr auto Pipeline_SID = "UniformBuffers_Pipeline"_sid;

    static constexpr auto VertexShader_SID = "UniformBuffers_vertex"_sid;
    static constexpr auto FragmentShader_SID = "UniformBuffers_fragment"_sid;

    static constexpr auto ViewProjectionMatricesBindingIndex = 0;
    static constexpr auto ModelInstanceMatricesBindingIndex = 1;

    static constexpr auto VertexPositionIndex = 0;

    static constexpr auto GridDimension = 5;
    static constexpr auto InstancesCount = GridDimension * GridDimension;


    namespace
    {
        struct Vertex
        {
            float position[3];
        };
        //--------------------------------------------------------------------------
    }

    using namespace Graphics::VKBits;


    UniformBuffersFrameListener::UniformBuffersFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Input::InputManager* inputManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _inputManager(inputManager)
        , _vertexBuffer(nullptr)
        , _uniformBuffersCommon()
        , _uniformBuffersInstanced()
        , _device(VK_NULL_HANDLE)
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(UniformBuffersFrameListener::_OnWindowResizeEvent))
        , _mouseButtonPressedHandler(_eventDispatcher, KMP_BIND(UniformBuffersFrameListener::_OnMouseButtonPressedEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(UniformBuffersFrameListener::_OnMouseScrollEvent))
        , _commonShaderData()
        , _instanceShaderData()
        , _dynamicAlignment(0ULL)
        , _camera({ 0.0f, 0.0f, -2.0f }, Graphics::Camera::Type::FirstPerson)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    UniformBuffersFrameListener::~UniformBuffersFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_Initialize()
    {
        _camera.SetMovementSpeed(0.0025f);
        _camera.SetRotationSpeed(0.1f);
        _camera.SetAspectRatio(float(_mainWindow.GetSize().x) / float(_mainWindow.GetSize().y));
        _camera.SetZNear(0.1f);
        _camera.SetZFar(10.0f);
        _camera.SetScale(16.0f);
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

        _InitializeInstances();
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_InitializeInstances()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        const auto& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();
        _device = vulkanDevice.GetVkDevice();
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const auto& vulkanRenderer = vulkanDevice.GetRenderer();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        const Vector<Vertex> vertices{
            { { -1.0f, -1.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f } }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        const auto vertexBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float3, 0 }
        });
        _vertexBuffer.reset(vulkanBufferCreator.CreateVertexBufferPtr({ VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize }));
        _vertexBuffer->AddLayout(vertexBufferLayout);

        {
            const auto copyCmd = vulkanRenderer.CreateCommandBuffer();
            copyCmd.Begin();
            vulkanRenderer.CopyBuffer(copyCmd, stagingBuffer, *_vertexBuffer.get(), 0, 0, vertexBufferSize);
            copyCmd.End();
            vulkanDevice.GetGraphicsQueue().SyncSubmit(copyCmd);
        }

        const auto minUboAlignment = vulkanContext.deviceProperties.limits.minUniformBufferOffsetAlignment;
        _dynamicAlignment = sizeof(Math::Mat4);
        if (minUboAlignment > 0)
        {
            _dynamicAlignment = (_dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }

        const auto instanceBufferSize = InstancesCount * _dynamicAlignment;
        _instanceShaderData.model = (Math::Mat4*)Utils::AlignedAlloc(instanceBufferSize, _dynamicAlignment);

        VkDescriptorSetLayoutBinding viewProjectionLayoutBinding{ ViewProjectionMatricesBindingIndex, VK_DescriptorType_UniformBuffer, 1, VK_ShaderStage_Vertex };
        VkDescriptorSetLayoutBinding instanceModelsLayoutBinding{ ModelInstanceMatricesBindingIndex, VK_DescriptorType_UniformBufferDynamic, 1, VK_ShaderStage_Vertex };
        const auto matricesLayout = descriptorSetManager.AddDescriptorSetLayout(MatricesDSLayout_SID, { viewProjectionLayoutBinding, instanceModelsLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(matricesLayout, MatricesDS_SID, 1, "per frame"_true);

        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            _uniformBuffersCommon.emplace_back(vulkanBufferCreator.CreateUniformBufferPtr({ 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, sizeof(CommonShaderData) }));
            _uniformBuffersCommon[i]->Map();
            descriptorSetManager.SetUniformBufferDescriptor(MatricesDS_SID, 0, "per frame"_true, i, *_uniformBuffersCommon[i].get(), _uniformBuffersCommon[i]->GetSize(), 0, ViewProjectionMatricesBindingIndex);

            _uniformBuffersInstanced.emplace_back(vulkanBufferCreator.CreateUniformBufferPtr({ 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, instanceBufferSize }));
            _uniformBuffersInstanced[i]->Map();
            descriptorSetManager.SetUniformBufferDynamicDescriptor(MatricesDS_SID, 0, "per frame"_true, i, _uniformBuffersInstanced[i].get()->GetVkBuffer(), _dynamicAlignment, ModelInstanceMatricesBindingIndex);
        }

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("uniform_buffers.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("uniform_buffers.frag.spv");
        const auto vertexShaderModule = vulkanDevice.CreateShaderModule(vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.CreateShaderModule(fragmentShaderPath);
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineParams.AddDescriptorSetLayout(matricesLayout);
        pipelineParams.SetInputAssembly(VK_Primitive_TriangleList, "primitive restart"_false);
        pipelineParams.SetPolygonMode(VK_Polygon_Fill);
        pipelineParams.SetCulling(VK_Cull_Back, VK_FrontFace_CounterClockwise);
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
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        vulkanDevice.AddGraphicsPipeline(Pipeline_SID, pipelineParams);
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_Finalize()
    {
        if (_instanceShaderData.model)
        {
            Utils::AlignedFree(_instanceShaderData.model);
        }

        _uniformBuffersInstanced.clear();
        _uniformBuffersCommon.clear();
        _vertexBuffer.reset();
    }
    //--------------------------------------------------------------------------

    bool UniformBuffersFrameListener::_OnWindowResizeEvent(Events::WindowResizeEvent& evt)
    {
        if (evt.GetWidth() > 0 && evt.GetHeight())
        {
            _camera.SetAspectRatio(float(evt.GetWidth()) / float(evt.GetHeight()));
        }
        return true;
    }
    //--------------------------------------------------------------------------

    bool UniformBuffersFrameListener::_OnMouseButtonPressedEvent(Events::MouseButtonPressEvent& evt)
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

    bool UniformBuffersFrameListener::_OnMouseScrollEvent(Events::MouseScrollEvent& evt)
    {
        _camera.SetScaleDelta(evt.GetYOffset());

        return true;
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::Update(float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {
        _camera.Update(frameTimestep);
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };
        const auto currentBufferIndex = vulkanGraphicsBackend.GetCurrentBufferIndex();
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        _commonShaderData.viewMatrix = _camera.GetViewMatrix();
        _commonShaderData.projectionMatrix = _camera.GetProjectionMatrix();
        _uniformBuffersCommon[currentBufferIndex]->CopyToMappedMemory(0, &_commonShaderData, sizeof(CommonShaderData));

        for (UInt32 r = 0; r < GridDimension; r++)
        {
            for (UInt32 c = 0; c < GridDimension; c++)
            {
                const auto index = r * GridDimension + c;
                auto* modelMatrix = (Math::Mat4*)((UInt64(_instanceShaderData.model) + (index * _dynamicAlignment)));
                const auto position = Math::Vec3F(c * 2.5f - 5.0f, r * 2.5 - 5.0f, 0.0);
                *modelMatrix = glm::translate(Math::Mat4(1.0f), position);
            }
        }
        _uniformBuffersInstanced[currentBufferIndex]->CopyToMappedMemory(0, _instanceShaderData.model, InstancesCount * _dynamicAlignment);

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(0, { _vertexBuffer->GetVkBuffer() }, { VkDeviceSize{ 0 } });
        renderer.BeginRendering(drawArea);

        for (UInt32 i = 0; i < InstancesCount; i++)
        {
            const auto dynamicOffset = i * UInt32(_dynamicAlignment);
            renderer.BindDescriptorSets(Pipeline_SID, 0, {
                descriptorSetManager.GetDescriptorSet(MatricesDS_SID, 0, "per frame"_true)
            }, { dynamicOffset });

            renderer.Draw(3, 1, 0, 0);
        }
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}