#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/orthographic_camera.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Input/input_manager.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class UniformBuffersFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(UniformBuffersFrameListener)
        KMP_DISABLE_COPY_MOVE(UniformBuffersFrameListener)

    public:
        UniformBuffersFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Input::InputManager* inputManager);
        ~UniformBuffersFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeInstances();
        void _Finalize();

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnMouseButtonPressedEvent(Events::MouseButtonPressEvent& evt);
        bool _OnMouseScrollEvent(Events::MouseScrollEvent& evt);

    private:
        struct CommonShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
        };
        struct InstanceShaderData
        {
            Math::Mat4* model = nullptr;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        Vector<UPtr<Graphics::VulkanBuffer>> _uniformBuffersCommon;
        Vector<UPtr<Graphics::VulkanBuffer>> _uniformBuffersInstanced;
        VkDevice _device;

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> _mouseButtonPressedHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
        CommonShaderData _commonShaderData;
        InstanceShaderData _instanceShaderData;
        size_t _dynamicAlignment;

        Graphics::OrthographicCamera _camera;
    };
    //--------------------------------------------------------------------------
}