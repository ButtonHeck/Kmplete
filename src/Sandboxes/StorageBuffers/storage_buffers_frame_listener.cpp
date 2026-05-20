#include "storage_buffers_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/memory_utils.h"
#include "Kmplete/Core/rng.h"
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

#include <numeric>


namespace Kmplete
{
    static constexpr auto DSLayout_SID = "DSLayout"_sid;
    static constexpr auto DS_SID = "DS"_sid;

    static constexpr auto Pipeline_SID = "StorageBuffers_Pipeline"_sid;

    static constexpr auto VertexShader_SID = "StorageBuffers_vertex"_sid;
    static constexpr auto FragmentShader_SID = "StorageBuffers_fragment"_sid;

    static constexpr auto MatricesBindingIndex = 0;
    static constexpr auto ColorsBindingIndex = 1;

    static constexpr auto VertexPositionIndex = 0;


    namespace
    {
        struct Vertex
        {
            float position[3];
        };
        //--------------------------------------------------------------------------
    }

    using namespace Graphics::VKBits;


    StorageBuffersFrameListener::StorageBuffersFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Input::InputManager* inputManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _inputManager(inputManager)
        , _vertexBuffer(nullptr)
        , _indexBuffer(nullptr)
        , _matricesStorageBuffers()
        , _colorsStorageBuffers()
        , _device(VK_NULL_HANDLE)
        , _indexCount(0)
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(StorageBuffersFrameListener::_OnWindowResizeEvent))
        , _mouseButtonPressedHandler(_eventDispatcher, KMP_BIND(StorageBuffersFrameListener::_OnMouseButtonPressedEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(StorageBuffersFrameListener::_OnMouseScrollEvent))
        , _matricesShaderData()
        , _colorsShaderData()
        , _dynamicAlignment(0ULL)
        , _colorRandomizingTimer(1000)
        , _camera(Graphics::Camera::Type::FirstPerson, 75.0f)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    StorageBuffersFrameListener::~StorageBuffersFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void StorageBuffersFrameListener::_Initialize()
    {
        _camera.SetMovementSpeed(0.025f);
        _camera.SetRotationSpeed(0.1f);
        _camera.SetAspectRatio(float(_mainWindow.GetSize().x) / float(_mainWindow.GetSize().y));
        _camera.SetZNear(0.1f);
        _camera.SetZFar(160.0f);

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

        _InitializeCubes();
    }
    //--------------------------------------------------------------------------

    void StorageBuffersFrameListener::_InitializeCubes()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();
        const auto& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();
        _device = vulkanDevice.GetVkDevice();
        const auto& vulkanBufferCreator = vulkanDevice.GetVulkanBufferCreatorDelegate();
        const auto& vulkanRenderer = vulkanDevice.GetRenderer();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        const Vector<Vertex> vertices{
            { { -0.5f, -0.5f,  0.5f } }, // 0: Bottom-left
            { {  0.5f, -0.5f,  0.5f } }, // 1: Bottom-right
            { {  0.5f,  0.5f,  0.5f } }, // 2: Top-right
            { { -0.5f,  0.5f,  0.5f } }, // 3: Top-left

            { { -0.5f, -0.5f, -0.5f } }, // 4: Bottom-left
            { {  0.5f, -0.5f, -0.5f } }, // 5: Bottom-right
            { {  0.5f,  0.5f, -0.5f } }, // 6: Top-right
            { { -0.5f,  0.5f, -0.5f } }  // 7: Top-left
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        const Vector<UInt32> indices{ 
            0, 1, 2,  2, 3, 0,
            // Right face
            1, 5, 6,  6, 2, 1,
            // Back face
            5, 4, 7,  7, 6, 5,
            // Left face
            4, 0, 3,  3, 7, 4,
            // Bottom face
            4, 5, 1,  1, 0, 4,
            // Top face
            3, 2, 6,  6, 7, 3
        };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferCreator.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize + indexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)indices.data(), indexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        const auto vertexBufferLayout = Graphics::BufferLayout({
            Graphics::BufferElement{ Graphics::ShaderDataType::Float3, 0 }
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

        const auto minUboAlignment = vulkanContext.deviceProperties.limits.minStorageBufferOffsetAlignment;
        _dynamicAlignment = sizeof(Math::Vec4F);
        if (minUboAlignment > 0)
        {
            _dynamicAlignment = (_dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }

        const auto colorsInstanceBufferSize = ColorsInstancesCount * _dynamicAlignment;
        _colorsShaderData.color = (Math::Vec4F*)Utils::AlignedAlloc(colorsInstanceBufferSize, _dynamicAlignment);

        VkDescriptorSetLayoutBinding matricesLayoutBinding{ MatricesBindingIndex, VK_DescriptorType_StorageBuffer, 1, VK_ShaderStage_Vertex };
        VkDescriptorSetLayoutBinding colorsLayoutBinding{ ColorsBindingIndex, VK_DescriptorType_StorageBufferDynamic, 1, VK_ShaderStage_Vertex };
        const auto DSLayout = descriptorSetManager.AddDescriptorSetLayout(DSLayout_SID, { matricesLayoutBinding, colorsLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(DSLayout, DS_SID, 1, "per frame"_true);

        RNGFloat rng(-80.0f, 80.0f);
        RNGFloat colorRng(0.0f, 1.0f);
        for (auto m = 0; m < InstancesCount; m++)
        {
            auto& modelMatrix = _matricesShaderData.models[m];
            modelMatrix = glm::translate(Math::Mat4(1.0f), Math::Vec3F(rng.Generate(), rng.Generate(), rng.Generate()));
        }
        for (auto c = 0; c < ColorsInstancesCount; c++)
        {
            auto* color = (Math::Vec4F*)((UInt64(_colorsShaderData.color) + (c * _dynamicAlignment)));
            color->r = colorRng.Generate();
            color->g = colorRng.Generate();
            color->b = colorRng.Generate();
            color->a = 1.0f;
        }
        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            _matricesStorageBuffers.emplace_back(vulkanBufferCreator.CreateStorageBufferPtr({ 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, sizeof(MatricesShaderData) }));
            _matricesStorageBuffers[i]->Map();
            _matricesStorageBuffers[i]->CopyToMappedMemory(sizeof(Math::Mat4) * 2, _matricesShaderData.models.data(), sizeof(_matricesShaderData.models));
            descriptorSetManager.SetStorageBufferDescriptor(DS_SID, 0, "per frame"_true, i, *_matricesStorageBuffers[i].get(), _matricesStorageBuffers[i]->GetSize(), 0, MatricesBindingIndex);

            _colorsStorageBuffers.emplace_back(vulkanBufferCreator.CreateStorageBufferPtr({ 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, colorsInstanceBufferSize }));
            _colorsStorageBuffers[i]->Map();
            _colorsStorageBuffers[i]->CopyToMappedMemory(0, _colorsShaderData.color, ColorsInstancesCount * _dynamicAlignment);
            descriptorSetManager.SetStorageBufferDynamicDescriptor(DS_SID, 0, "per frame"_true, i, _colorsStorageBuffers[i].get()->GetVkBuffer(), _dynamicAlignment, 0, ColorsBindingIndex);
        }

        const auto vertexShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("storage_buffers.vert.spv");
        const auto fragmentShaderPath = String(KMP_SANDBOX_RESOURCES_FOLDER).append("storage_buffers.frag.spv");
        const auto vertexShaderModule = vulkanDevice.CreateShaderModule(vertexShaderPath);
        const auto fragmentShaderModule = vulkanDevice.CreateShaderModule(fragmentShaderPath);
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Vertex, "main"),
            fragmentShaderModule.GetShaderStageCreateInfo(VK_ShaderStage_Fragment, "main")
        };

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineParams.AddDescriptorSetLayout(DSLayout);
        pipelineParams.SetInputAssembly(VK_Primitive_TriangleList, "primitive restart"_false);
        pipelineParams.SetPolygonMode(VK_Polygon_Fill);
        pipelineParams.SetCulling(VK_Cull_None, VK_FrontFace_CounterClockwise);
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

        std::iota(_colorsIndices.begin(), _colorsIndices.end(), 0);

        _colorRandomizingTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void StorageBuffersFrameListener::_Finalize()
    {
        if (_colorsShaderData.color)
        {
            Utils::AlignedFree(_colorsShaderData.color);
        }

        _colorsStorageBuffers.clear();
        _matricesStorageBuffers.clear();
        _indexBuffer.reset();
        _vertexBuffer.reset();
    }
    //--------------------------------------------------------------------------

    bool StorageBuffersFrameListener::_OnWindowResizeEvent(Events::WindowResizeEvent& evt)
    {
        if (evt.GetWidth() > 0 && evt.GetHeight())
        {
            _camera.SetAspectRatio(float(evt.GetWidth()) / float(evt.GetHeight()));
        }
        return true;
    }
    //--------------------------------------------------------------------------

    bool StorageBuffersFrameListener::_OnMouseButtonPressedEvent(Events::MouseButtonPressEvent& evt)
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

    bool StorageBuffersFrameListener::_OnMouseScrollEvent(Events::MouseScrollEvent& evt)
    {
        _camera.SetFOVDelta(evt.GetYOffset());

        return true;
    }
    //--------------------------------------------------------------------------

    void StorageBuffersFrameListener::Update(float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {
        _camera.Update(frameTimestep);

        if (_colorRandomizingTimer.ReachedTimeout())
        {
            _colorRandomizingTimer.Mark();
            MersenneTwister32 mt32;
            std::shuffle(_colorsIndices.begin(), _colorsIndices.end(), mt32.engine);
        }
    }
    //--------------------------------------------------------------------------

    void StorageBuffersFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };
        const auto currentBufferIndex = vulkanGraphicsBackend.GetCurrentBufferIndex();
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        _matricesShaderData.viewMatrix = _camera.GetViewMatrix();
        _matricesShaderData.projectionMatrix = _camera.GetProjectionMatrix();
        _matricesStorageBuffers[currentBufferIndex]->CopyToMappedMemory(0, &_matricesShaderData, sizeof(Math::Mat4) * 2);

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(0, { _vertexBuffer->GetVkBuffer() }, { VkDeviceSize{ 0 } });
        renderer.BindIndexBuffer(*_indexBuffer.get());
        renderer.BeginRendering(drawArea);

        const auto instancesPerColor = InstancesCount / ColorsInstancesCount;
        for (auto i = 0; i < ColorsInstancesCount; i++)
        {
            const auto dynamicOffset = _colorsIndices[i] * UInt32(_dynamicAlignment);
            renderer.BindDescriptorSets(Pipeline_SID, 0, {
                descriptorSetManager.GetDescriptorSet(DS_SID, 0, "per frame"_true)
            }, { dynamicOffset });

            renderer.DrawIndexed(_indexCount, instancesPerColor, 0, 0, instancesPerColor * i);
        }

        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------
}