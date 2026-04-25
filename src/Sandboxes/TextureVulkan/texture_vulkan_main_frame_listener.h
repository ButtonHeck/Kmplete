#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/orthographic_camera.h"
#include "Kmplete/Graphics/perspective_camera.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Input/input_manager.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Assets
    {
        class AssetsManager;
    }


#define USE_ORTHOGRAPHIC_CAMERA false

    class MainFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(MainFrameListener)
        KMP_DISABLE_COPY_MOVE(MainFrameListener)

    public:
        MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, Input::InputManager* inputManager);
        ~MainFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeTextureQuad();
        void _InitializeImGui(float dpiScale);
        void _Finalize();

        void _RenderTextureQuad();
        void _RenderImGui();

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnMouseButtonPressedEvent(Events::MouseButtonPressEvent& evt);
        bool _OnMouseScrollEvent(Events::MouseScrollEvent& evt);

    private:
        struct MatrixShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
            Math::Mat4 modelMatrix;
            float lodBias = 0.0f;
            int tiling = 1;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        UPtr<Graphics::VulkanBuffer> _indexBuffer;
        Vector<UPtr<Graphics::VulkanBuffer>> _uniformBuffers;
        UInt32 _indexCount;
        VkDevice _device;

        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> _mouseButtonPressedHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
        MatrixShaderData _matrixShaderData;

#if USE_ORTHOGRAPHIC_CAMERA
        Graphics::OrthographicCamera _camera;
#else
        Graphics::PerspectiveCamera _camera;
#endif
    };
    //--------------------------------------------------------------------------
}