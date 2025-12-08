#pragma once

#include "shared_state.h"

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/key_events.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Input/input_manager.h"


namespace Kmplete
{
    namespace Events
    {
        struct CustomEvent : public Event
        {
            EVENT_CLASS_TYPE("CustomEvent")

            CustomEvent() = default;
        };
        //--------------------------------------------------------------------------
    }


    class TestMainFrameListener : public FrameListener
    {
    public:
        TestMainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, GraphicsBackend* graphicsBackend, WindowBackend* windowBackend, Input::InputManager* inputManager);

        void Initialize();

        void Update(float /*frameTimestep*/, bool /*applicationIsIconified*/) override;
        void Render() override;

        void SetCustomIconFromFilepath();
        void SetCustomIconFromBuffer();
        void SetCustomCursor();
        void SwitchFonts();

        bool IsKeyPressEventInvoked() const { return _keyPressEventInvoked; }
        bool IsKeyReleaseEventInvoked() const { return _keyReleaseEventInvoked; }
        bool IsKeyCharEventInvoked() const { return _keyCharEventInvoked; }

        bool IsMouseMoveEventInvoked() const { return _mouseMoveEventInvoked; }
        bool IsMouseScrollEventInvoked() const { return _mouseScrollEventInvoked; }
        bool IsMouseButtonPressEventInvoked() const { return _mouseButtonPressEventInvoked; }
        bool IsMouseButtonReleaseEventInvoked() const { return _mouseButtonReleaseEventInvoked; }

        bool IsWindowCloseEventInvoked() const { return _windowCloseEventInvoked; }
        bool IsWindowMoveEventInvoked() const { return _windowMoveEventInvoked; }
        bool IsWindowResizeEventInvoked() const { return _windowResizeEventInvoked; }
        bool IsWindowFocusEventInvoked() const { return _windowFocusEventInvoked; }
        bool IsWindowIconifyEventInvoked() const { return _windowIconifyEventInvoked; }
        bool IsWindowFramebufferRefreshEventInvoked() const { return _windowFramebufferRefreshEventInvoked; }
        bool IsWindowFramebufferResizeEventInvoked() const { return _windowFramebufferResizeEventInvoked; }

        bool IsCustomEventInvoked() const { return _customEventInvoked; }

        bool MousePositionIsNotZero() const;
        bool DPIIsNotZero() const;
        bool DPIScaleIsNotZero() const;
        bool DefaultSizeIsNotZero() const;
        bool DefaultWindowedSizeIsNotZero() const;

        bool DuplicateSidFrameListenerCheck() const
        {
            return !_duplicateSidFrameListenerCheckActivated || _duplicateSidFrameListenerCheckSuccess;
        }

        bool DuplicatePriorityFrameListenerCheck() const
        {
            return !_duplicatePriorityFrameListenerCheckActivated || _duplicatePriorityFrameListenerCheckSuccess;
        }

        void SetMouseClickHandlerActive(bool active);

    protected:
        KMP_NODISCARD virtual bool OnKeyPressEvent(Events::KeyPressEvent&) { _keyPressEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(Events::KeyReleaseEvent&) { _keyReleaseEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(Events::KeyCharEvent&) { _keyCharEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(Events::MouseMoveEvent&) { _mouseMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(Events::MouseScrollEvent&) { _mouseScrollEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(Events::MouseButtonPressEvent& evt);

        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(Events::MouseButtonReleaseEvent&) { _mouseButtonReleaseEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnWindowMoveEvent(Events::WindowMoveEvent&) { _windowMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(Events::WindowResizeEvent&) { _windowResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(Events::WindowFocusEvent&) { _windowFocusEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(Events::WindowIconifyEvent&) { _windowIconifyEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(Events::WindowFramebufferRefreshEvent&);
        KMP_NODISCARD virtual bool OnWindowContentScaleEvent(Events::WindowContentScaleEvent& evt);
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(Events::WindowCloseEvent&);

        KMP_NODISCARD virtual bool OnCustomEvent(Events::CustomEvent&) { _customEventInvoked = true; return true; }

    private:
        SharedState& _sharedState;
        Window& _mainWindow;
        FrameListenerManager& _frameListenerManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        Assets::AssetsManager* _assetsManager;
        GraphicsBackend* _graphicsBackend;
        WindowBackend* _windowBackend;
        Input::InputManager* _inputManager;

        Events::EventHandlerGuard<Events::KeyPressEvent> _keyPressHandler;
        Events::EventHandlerGuard<Events::KeyReleaseEvent> _keyReleaseHandler;
        Events::EventHandlerGuard<Events::KeyCharEvent> _keyCharHandler;

        Events::EventHandlerGuard<Events::MouseMoveEvent> _mouseMoveHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> _mouseButtonPressedHandler;
        Events::EventHandlerGuard<Events::MouseButtonReleaseEvent> _mouseButtonReleasedHandler;

        Events::EventHandlerGuard<Events::WindowCloseEvent> _windowCloseHandler;
        Events::EventHandlerGuard<Events::WindowMoveEvent> _windowMoveHandler;
        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::WindowFocusEvent> _windowFocusHandler;
        Events::EventHandlerGuard<Events::WindowIconifyEvent> _windowIconifyHandler;
        Events::EventHandlerGuard<Events::WindowFramebufferRefreshEvent> _windowFramebufferRefreshHandler;
        Events::EventHandlerGuard<Events::WindowFramebufferResizeEvent> _windowFramebufferResizeHandler;
        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;

        Events::EventHandlerGuard<Events::CustomEvent> _customEventHandler;

        bool _switchFontRequested = false;
        bool _useDefaultFont = true;

        bool _keyPressEventInvoked = false;
        bool _keyReleaseEventInvoked = false;
        bool _keyCharEventInvoked = false;

        bool _mouseMoveEventInvoked = false;
        bool _mouseScrollEventInvoked = false;
        bool _mouseButtonPressEventInvoked = false;
        bool _mouseButtonReleaseEventInvoked = false;

        bool _windowCloseEventInvoked = false;
        bool _windowMoveEventInvoked = false;
        bool _windowResizeEventInvoked = false;
        bool _windowFocusEventInvoked = false;
        bool _windowIconifyEventInvoked = false;
        bool _windowFramebufferRefreshEventInvoked = false;
        bool _windowFramebufferResizeEventInvoked = false;

        bool _customEventInvoked = false;

        bool _duplicateSidFrameListenerCheckActivated = false;
        bool _duplicateSidFrameListenerCheckSuccess = false;
        bool _duplicatePriorityFrameListenerCheckActivated = false;
        bool _duplicatePriorityFrameListenerCheckSuccess = false;
    };
    //--------------------------------------------------------------------------
}