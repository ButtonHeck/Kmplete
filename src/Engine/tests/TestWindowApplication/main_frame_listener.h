#pragma once

#include "shared_state.h"
#include "events.h"

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
    class MainFrameListener : public FrameListener
    {
    public:
        MainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, 
                          GraphicsBackend* graphicsBackend, WindowBackend* windowBackend, Input::InputManager* inputManager);

        void Initialize();

        void Update(float /*frameTimestep*/, bool /*applicationIsIconified*/) override;

        void Render() override;
        void RenderEventsWindow(int windowFlags);
        void RenderControlsWindow(int windowFlags);
        void RenderInfoWindow(int windowFlags);
        void RenderFrameListenersWindow(int windowFlags);
        void RenderActionEventsEmulatorWindow(int windowFlags);

        void SetCustomIconFromFilepath();
        void SetCustomIconFromBuffer();
        void SetCustomCursor();
        void SwitchFonts();

        bool IsKeyPressEventInvoked() const { return _keyPressEventInvokedCount > 0; }
        bool IsKeyReleaseEventInvoked() const { return _keyReleaseEventInvokedCount > 0; }
        bool IsKeyCharEventInvoked() const { return _keyCharEventInvokedCount > 0; }

        bool IsMouseMoveEventInvoked() const { return _mouseMoveEventInvokedCount > 0; }
        bool IsMouseScrollEventInvoked() const { return _mouseScrollEventInvokedCount > 0; }
        bool IsMouseButtonPressEventInvoked() const { return _mouseButtonPressEventInvokedCount > 0; }
        bool IsMouseButtonReleaseEventInvoked() const { return _mouseButtonReleaseEventInvokedCount > 0; }

        bool IsWindowCloseEventInvoked() const { return _windowCloseEventInvokedCount > 0; }
        bool IsWindowMoveEventInvoked() const { return _windowMoveEventInvokedCount > 0; }
        bool IsWindowResizeEventInvoked() const { return _windowResizeEventInvokedCount > 0; }
        bool IsWindowFocusEventInvoked() const { return _windowFocusEventInvokedCount > 0; }
        bool IsWindowIconifyEventInvoked() const { return _windowIconifyEventInvokedCount > 0; }
        bool IsWindowFramebufferRefreshEventInvoked() const { return _windowFramebufferRefreshEventInvokedCount > 0; }
        bool IsWindowFramebufferResizeEventInvoked() const { return _windowFramebufferResizeEventInvokedCount > 0; }

        bool IsCustomEventInvoked() const { return _customEventInvokedCount > 0; }

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

        bool ActionDefaultTagCallbackDoubleRegistrationCheck() const
        {
            return _actionDefaultTagCallbackDoubleRegistrationCheck;
        }

        bool ActionNonDefaultTagCallbackDoubleRegistrationCheck() const
        {
            return _actionNonDefaultTagCallbackDoubleRegistrationCheck;
        }

        bool UnmapUnregisteredActionCheck() const
        {
            return _unmapUnregisteredActionCheck;
        }

        bool InputToActionDoubleRegistrationCheck() const
        {
            return _inputToActionDoubleRegistrationCheck;
        }

        bool UnmapInvalidInputFromActionCheck() const
        {
            return _unmapInvalidInputFromActionCheck;
        }

        bool RemapInvalidInputToActionCheck() const
        {
            return _remapInvalidInputToActionCheck;
        }

        void SetMouseClickHandlerActive(bool active);

    protected:
        KMP_NODISCARD virtual bool OnKeyPressEvent(Events::KeyPressEvent&);
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(Events::KeyReleaseEvent&);
        KMP_NODISCARD virtual bool OnKeyCharEvent(Events::KeyCharEvent&) { _keyCharEventInvokedCount++; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(Events::MouseMoveEvent&) { _mouseMoveEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(Events::MouseScrollEvent&) { _mouseScrollEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(Events::MouseButtonPressEvent& evt);
        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(Events::MouseButtonReleaseEvent&);

        KMP_NODISCARD virtual bool OnWindowMoveEvent(Events::WindowMoveEvent&) { _windowMoveEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(Events::WindowResizeEvent&) { _windowResizeEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(Events::WindowFocusEvent&) { _windowFocusEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(Events::WindowIconifyEvent&) { _windowIconifyEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(Events::WindowFramebufferRefreshEvent&);
        KMP_NODISCARD virtual bool OnWindowContentScaleEvent(Events::WindowContentScaleEvent& evt);
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvokedCount++; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(Events::WindowCloseEvent&);

        KMP_NODISCARD virtual bool OnCustomEvent(Events::CustomEvent&) { _customEventInvokedCount++; return true; }

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

        int _keyPressEventInvokedCount = 0;
        int _keyReleaseEventInvokedCount = 0;
        int _keyCharEventInvokedCount = 0;

        int _mouseMoveEventInvokedCount = 0;
        int _mouseScrollEventInvokedCount = 0;
        int _mouseButtonPressEventInvokedCount = 0;
        int _mouseButtonReleaseEventInvokedCount = 0;

        int _windowCloseEventInvokedCount = 0;
        int _windowMoveEventInvokedCount = 0;
        int _windowResizeEventInvokedCount = 0;
        int _windowFocusEventInvokedCount = 0;
        int _windowIconifyEventInvokedCount = 0;
        int _windowFramebufferRefreshEventInvokedCount = 0;
        int _windowFramebufferResizeEventInvokedCount = 0;

        int _customEventInvokedCount = 0;

        bool _duplicateSidFrameListenerCheckActivated = false;
        bool _duplicateSidFrameListenerCheckSuccess = false;
        bool _duplicatePriorityFrameListenerCheckActivated = false;
        bool _duplicatePriorityFrameListenerCheckSuccess = false;

        HashSet<Input::KeyCode> _keysPressed;

        bool _mouseButtonHandlersColoring = false;

        Math::Point2I _emulatorPlayerPos = Math::Point2I(0, 0);
        int _emulatorMoveWASD = 1;
        bool _actionDefaultTagCallbackDoubleRegistrationCheck = false;
        bool _actionNonDefaultTagCallbackDoubleRegistrationCheck = false;
        bool _unmapUnregisteredActionCheck = false;
        bool _inputToActionDoubleRegistrationCheck = false;
        bool _unmapInvalidInputFromActionCheck = false;
        bool _remapInvalidInputToActionCheck = false;
    };
    //--------------------------------------------------------------------------
}