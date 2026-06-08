#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/orthographic_camera.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Input/input_manager.h"
#include "Kmplete/Utils/memory_utils.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class UniformBuffersFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(UniformBuffersFrameListener)

    public:
        UniformBuffersFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Input::InputManager* inputManager);
        ~UniformBuffersFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeCamera();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _Finalize();

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnMouseScrollEvent(Events::MouseScrollEvent& evt);

    private:
        struct CommonShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
        };
        struct InstanceShaderData
        {
            InstanceShaderData(std::size_t size, std::size_t alignment)
                : model((Math::Mat4*)Utils::AlignedAlloc(size, alignment))
            {}

            ~InstanceShaderData()
            {
                Utils::AlignedFree(model);
            }

            Math::Mat4* model = nullptr;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        Vector<UPtr<Graphics::VulkanBuffer>> _uniformBuffersCommon;
        Vector<UPtr<Graphics::VulkanBuffer>> _uniformBuffersInstanced;
        CommonShaderData _commonShaderData;
        UPtr<InstanceShaderData> _instanceShaderData;
        size_t _dynamicAlignment;
        Vector<float> _rotationsAngles;
        Graphics::OrthographicCamera _camera;

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
    };
    //--------------------------------------------------------------------------
}