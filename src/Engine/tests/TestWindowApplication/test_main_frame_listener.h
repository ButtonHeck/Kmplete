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
        KMP_NODISCARD virtual bool OnKeyPressEvent(Events::KeyPressEvent&);
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(Events::KeyReleaseEvent&);
        KMP_NODISCARD virtual bool OnKeyCharEvent(Events::KeyCharEvent&) { _keyCharEventInvoked = true; _keyCharEventInvokedCount++; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(Events::MouseMoveEvent&) { _mouseMoveEventInvoked = true; _mouseMoveEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(Events::MouseScrollEvent&) { _mouseScrollEventInvoked = true; _mouseScrollEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(Events::MouseButtonPressEvent& evt);
        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(Events::MouseButtonReleaseEvent&);

        KMP_NODISCARD virtual bool OnWindowMoveEvent(Events::WindowMoveEvent&) { _windowMoveEventInvoked = true; _windowMoveEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(Events::WindowResizeEvent&) { _windowResizeEventInvoked = true; _windowResizeEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(Events::WindowFocusEvent&) { _windowFocusEventInvoked = true; _windowFocusEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(Events::WindowIconifyEvent&) { _windowIconifyEventInvoked = true; _windowIconifyEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(Events::WindowFramebufferRefreshEvent&);
        KMP_NODISCARD virtual bool OnWindowContentScaleEvent(Events::WindowContentScaleEvent& evt);
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvoked = true; _windowFramebufferResizeEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(Events::WindowCloseEvent&);

        KMP_NODISCARD virtual bool OnCustomEvent(Events::CustomEvent&) { _customEventInvoked = true; _customEventInvokedCount++; return true; }

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
        int _keyPressEventInvokedCount = 0;
        bool _keyReleaseEventInvoked = false;
        int _keyReleaseEventInvokedCount = 0;
        bool _keyCharEventInvoked = false;
        int _keyCharEventInvokedCount = 0;

        bool _mouseMoveEventInvoked = false;
        int _mouseMoveEventInvokedCount = 0;
        bool _mouseScrollEventInvoked = false;
        int _mouseScrollEventInvokedCount = 0;
        bool _mouseButtonPressEventInvoked = false;
        int _mouseButtonPressEventInvokedCount = 0;
        bool _mouseButtonReleaseEventInvoked = false;
        int _mouseButtonReleaseEventInvokedCount = 0;

        bool _windowCloseEventInvoked = false;
        int _windowCloseEventInvokedCount = 0;
        bool _windowMoveEventInvoked = false;
        int _windowMoveEventInvokedCount = 0;
        bool _windowResizeEventInvoked = false;
        int _windowResizeEventInvokedCount = 0;
        bool _windowFocusEventInvoked = false;
        int _windowFocusEventInvokedCount = 0;
        bool _windowIconifyEventInvoked = false;
        int _windowIconifyEventInvokedCount = 0;
        bool _windowFramebufferRefreshEventInvoked = false;
        int _windowFramebufferRefreshEventInvokedCount = 0;
        bool _windowFramebufferResizeEventInvoked = false;
        int _windowFramebufferResizeEventInvokedCount = 0;

        bool _customEventInvoked = false;
        int _customEventInvokedCount = 0;

        bool _duplicateSidFrameListenerCheckActivated = false;
        bool _duplicateSidFrameListenerCheckSuccess = false;
        bool _duplicatePriorityFrameListenerCheckActivated = false;
        bool _duplicatePriorityFrameListenerCheckSuccess = false;

        HashSet<Input::KeyCode> _keysPressed;

        bool _mouseButtonHandlersColoring = false;
    };
    //--------------------------------------------------------------------------
}