#include "uniform_buffers_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/Command/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
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

    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "UniformBuffers_Pipeline"_sid;

    static constexpr auto VertexShader_SID = "UniformBuffers_vertex"_sid;
    static constexpr auto FragmentShader_SID = "UniformBuffers_fragment"_sid;

    static constexpr auto ViewProjectionMatricesBindingIndex = 0;
    static constexpr auto ModelInstanceMatricesBindingIndex = 1;

    static constexpr auto VertexBufferBinding = 0;

    static constexpr auto VertexPositionAttributeIndex = 0;

    static constexpr auto GridDimension = 5;
    static constexpr auto InstancesCount = GridDimension * GridDimension;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffers"_sid;
    static constexpr auto UniformBufferCommon_SID = "uniform_buffer_common"_sid;
    static constexpr auto UniformBufferInstanced_SID = "uniform_buffer_instanced"_sid;


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
        , _commonShaderData()
        , _instanceShaderData(nullptr)
        , _dynamicAlignment(0ULL)
        , _rotationsAngles()
        , _camera({ 0.0f, 0.0f, -2.0f }, Graphics::Camera::Type::FirstPerson)
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(UniformBuffersFrameListener::_OnWindowResizeEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(UniformBuffersFrameListener::_OnMouseScrollEvent))
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeCamera();
        _InitializeBuffers(vulkanDevice);
        _InitializeUniformBuffers(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_InitializeCamera()
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
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanRenderer = vulkanDevice.GetRenderer();

        const Vector<Vertex> vertices{
            { { -1.0f, -1.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f } }
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
        auto vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float3, VertexPositionAttributeIndex }
        });

        vulkanRenderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        const auto minUboAlignment = vulkanContext.deviceProperties.limits.minUniformBufferOffsetAlignment;
        _dynamicAlignment = sizeof(Math::Mat4);
        if (minUboAlignment > 0)
        {
            _dynamicAlignment = (_dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }

        const auto instanceBufferSize = InstancesCount * _dynamicAlignment;
        _instanceShaderData.reset(new InstanceShaderData(instanceBufferSize, _dynamicAlignment));

        _rotationsAngles.resize(InstancesCount, 0.0f);

        VkDescriptorSetLayoutBinding viewProjectionLayoutBinding{ ViewProjectionMatricesBindingIndex, VK_DescriptorType_UniformBuffer, 1, VK_ShaderStage_Vertex };
        VkDescriptorSetLayoutBinding instanceModelsLayoutBinding{ ModelInstanceMatricesBindingIndex, VK_DescriptorType_UniformBufferDynamic, 1, VK_ShaderStage_Vertex };
        const auto matricesLayout = descriptorSetManager.AddDescriptorSetLayout(MatricesDSLayout_SID, { viewProjectionLayoutBinding, instanceModelsLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(matricesLayout, MatricesDS_SID, 1, "per frame"_true);

        vulkanBufferManager.CreateUniformBuffer(UniformBufferCommon_SID, { 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, sizeof(CommonShaderData) }, "per frame"_true);
        vulkanBufferManager.CreateUniformBuffer(UniformBufferInstanced_SID, { 0, VK_Memory_HostVisible | VK_Memory_HostCoherent, instanceBufferSize }, "per frame"_true);
        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            auto uniformBufferCommon = vulkanBufferManager.GetBuffer(UniformBufferCommon_SID, i);
            uniformBufferCommon->Map();
            descriptorSetManager.SetUniformBufferDescriptor(MatricesDS_SID, 0, "per frame"_true, i, *uniformBufferCommon, uniformBufferCommon->GetSize(), 0, ViewProjectionMatricesBindingIndex);

            auto uniformBufferInstanced = vulkanBufferManager.GetBuffer(UniformBufferInstanced_SID, i);
            uniformBufferInstanced->Map();
            descriptorSetManager.SetUniformBufferDynamicDescriptor(MatricesDS_SID, 0, "per frame"_true, i, *uniformBufferInstanced, _dynamicAlignment, 0, ModelInstanceMatricesBindingIndex);
        }
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();

        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormat.format, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat, VK_ImageUsage_DepthStencilAttachment);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayoutWithSetsSids(PipelineLayout_SID, { MatricesDSLayout_SID });

        auto& shaderManager = vulkanDevice.GetShaderManager();
        shaderManager.AddShaderModules({
            { VertexShader_SID, String(KMP_SANDBOX_RESOURCES_FOLDER).append("uniform_buffers.vert.spv") },
            { FragmentShader_SID, String(KMP_SANDBOX_RESOURCES_FOLDER).append("uniform_buffers.frag.spv") }
        });
        const auto shaderStages = shaderManager.GetShaderStageCreateInfos({
            { VertexShader_SID, VK_ShaderStage_Vertex, "main" },
            { FragmentShader_SID, VK_ShaderStage_Fragment, "main" }
        });

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormat.format, Graphics::VKPresets::ColorBlendAttachmentState_NoBlend);
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID), VertexBufferBinding);
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "uniform_buffers_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::Update(float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {
        _camera.Update(frameTimestep);

        for (auto i = 0; i < InstancesCount; i++)
        {
            _rotationsAngles[i] += 0.0001f * i * frameTimestep;
        }
    }
    //--------------------------------------------------------------------------

    void UniformBuffersFrameListener::Render()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto drawArea = VkRect2D{ VkOffset2D{ .x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = VkViewport{ .x = 0, .y = 0, .width = float(_mainWindow.GetSize().x), .height = float(_mainWindow.GetSize().y), .minDepth = 0.0f, .maxDepth = 1.0f };
        const auto currentBufferIndex = vulkanGraphicsBackend.GetCurrentBufferIndex();
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        _commonShaderData.viewMatrix = _camera.GetViewMatrix();
        _commonShaderData.projectionMatrix = _camera.GetProjectionMatrix();
        vulkanBufferManager.GetBuffer(UniformBufferCommon_SID, currentBufferIndex)->CopyToMappedMemory(0, &_commonShaderData, sizeof(CommonShaderData));

        for (UInt32 r = 0; r < GridDimension; r++)
        {
            for (UInt32 c = 0; c < GridDimension; c++)
            {
                const auto index = r * GridDimension + c;
                auto* modelMatrix = (Math::Mat4*)((UInt64(_instanceShaderData->model) + (index * _dynamicAlignment)));
                const auto position = Math::Vec3F(c * 2.5f - 5.0f, r * 2.5 - 5.0f, 0.0);
                *modelMatrix = glm::translate(Math::IdentityMatrix, position);
                *modelMatrix = glm::rotate(*modelMatrix, _rotationsAngles[index], glm::vec3(0.0f, 0.0f, 1.0f));
            }
        }
        vulkanBufferManager.GetBuffer(UniformBufferInstanced_SID, currentBufferIndex)->CopyToMappedMemory(0, _instanceShaderData->model, InstancesCount * _dynamicAlignment);

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer() }, { VkDeviceSize{ 0 } });

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

        for (UInt32 i = 0; i < InstancesCount; i++)
        {
            const auto dynamicOffset = i * UInt32(_dynamicAlignment);
            renderer.BindDescriptorSets(PipelineLayout_SID, 0, {
                descriptorSetManager.GetDescriptorSet(MatricesDS_SID, 0, "per frame"_true)
            }, { dynamicOffset });

            renderer.Draw(3, 1, 0, 0);
        }
        renderer.EndRendering();
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

    bool UniformBuffersFrameListener::_OnMouseScrollEvent(Events::MouseScrollEvent& evt)
    {
        _camera.SetScaleDelta(evt.GetYOffset());

        return true;
    }
    //--------------------------------------------------------------------------
}