#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/orthographic_camera.h"
#include "Kmplete/Graphics/perspective_camera.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture.h"
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

    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


#define USE_ORTHOGRAPHIC_CAMERA false

    class TextureFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(TextureFrameListener)

    public:
        TextureFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, 
                             Assets::AssetsManager& assetsManager, Input::InputManager* inputManager);
        ~TextureFrameListener() = default;

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeCamera();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializeImGui(float dpiScale);

        void _RenderTextureQuad();
        void _RenderImGui();

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);
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

        UInt32 _indexCount;
        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        MatrixShaderData _matrixShaderData;

#if USE_ORTHOGRAPHIC_CAMERA
        Graphics::OrthographicCamera _camera;
#else
        Graphics::PerspectiveCamera _camera;
#endif

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> _mouseButtonPressedHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
    };
    //--------------------------------------------------------------------------
}