#pragma once

#include "shared_state.h"

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/ImGui/implementation.h"


namespace Kmplete
{
    class TestMainFrameListener : public FrameListener
    {
    public:
        TestMainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, GraphicsBackend* graphicsBackend, WindowBackend* windowBackend);

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
        bool IsWindowApplicationKeyPressEventInvoked() const { return _windowApplicationKeyPressEventInvoked; }
        bool IsWindowApplicationKeyReleaseEventInvoked() const { return _windowApplicationKeyReleaseEventInvoked; }
        bool IsWindowApplicationKeyCharEventInvoked() const { return _windowApplicationKeyCharEventInvoked; }

        bool IsMouseMoveEventInvoked() const { return _mouseMoveEventInvoked; }
        bool IsMouseScrollEventInvoked() const { return _mouseScrollEventInvoked; }
        bool IsMouseButtonPressEventInvoked() const { return _mouseButtonPressEventInvoked; }
        bool IsMouseButtonReleaseEventInvoked() const { return _mouseButtonReleaseEventInvoked; }
        bool IsWindowApplicationMouseMoveEventInvoked() const { return _windowApplicationMouseMoveEventInvoked; }
        bool IsWindowApplicationMouseScrollEventInvoked() const { return _windowApplicationMouseScrollEventInvoked; }
        bool IsWindowApplicationMouseButtonReleaseEventInvoked() const { return _windowApplicationMouseButtonReleaseEventInvoked; }

        bool IsWindowCloseEventInvoked() const { return _windowCloseEventInvoked; }
        bool IsWindowMoveEventInvoked() const { return _windowMoveEventInvoked; }
        bool IsWindowResizeEventInvoked() const { return _windowResizeEventInvoked; }
        bool IsWindowFocusEventInvoked() const { return _windowFocusEventInvoked; }
        bool IsWindowIconifyEventInvoked() const { return _windowIconifyEventInvoked; }
        bool IsWindowFramebufferRefreshEventInvoked() const { return _windowFramebufferRefreshEventInvoked; }
        bool IsWindowFramebufferResizeEventInvoked() const { return _windowFramebufferResizeEventInvoked; }
        bool IsWindowApplicationWindowCloseEventInvoked() const { return _windowApplicationWindowCloseEventInvoked; }
        bool IsWindowApplicationWindowMoveEventInvoked() const { return _windowApplicationWindowMoveEventInvoked; }
        bool IsWindowApplicationWindowResizeEventInvoked() const { return _windowApplicationWindowResizeEventInvoked; }
        bool IsWindowApplicationWindowFocusEventInvoked() const { return _windowApplicationWindowFocusEventInvoked; }
        bool IsWindowApplicationWindowIconifyEventInvoked() const { return _windowApplicationWindowIconifyEventInvoked; }
        bool IsWindowApplicationWindowFramebufferRefreshEventInvoked() const { return _windowApplicationWindowFramebufferRefreshEventInvoked; }
        bool IsWindowApplicationWindowFramebufferResizeEventInvoked() const { return _windowApplicationWindowFramebufferResizeEventInvoked; }

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

    protected:
        void OnEvent(Event& event) override;

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { _keyPressEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { _keyReleaseEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { _keyCharEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent&) { _mouseMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent&) { _mouseScrollEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& evt);

        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent&) { _mouseButtonReleaseEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent&) { _windowMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent&) { _windowResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent&) { _windowFocusEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent&) { _windowIconifyEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&);
        KMP_NODISCARD virtual bool OnWindowContentScaleEvent(WindowContentScaleEvent& evt);
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent&);

    private:
        SharedState& _sharedState;
        Window& _mainWindow;
        FrameListenerManager& _frameListenerManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        Assets::AssetsManager* _assetsManager;
        GraphicsBackend* _graphicsBackend;
        WindowBackend* _windowBackend;

        bool _switchFontRequested = false;
        bool _useDefaultFont = true;

        bool _keyPressEventInvoked = false;
        bool _keyReleaseEventInvoked = false;
        bool _keyCharEventInvoked = false;
        bool _windowApplicationKeyPressEventInvoked = false;
        bool _windowApplicationKeyReleaseEventInvoked = false;
        bool _windowApplicationKeyCharEventInvoked = false;

        bool _mouseMoveEventInvoked = false;
        bool _mouseScrollEventInvoked = false;
        bool _mouseButtonPressEventInvoked = false;
        bool _mouseButtonReleaseEventInvoked = false;
        bool _windowApplicationMouseMoveEventInvoked = false;
        bool _windowApplicationMouseScrollEventInvoked = false;
        bool _windowApplicationMouseButtonReleaseEventInvoked = false;

        bool _windowCloseEventInvoked = false;
        bool _windowMoveEventInvoked = false;
        bool _windowResizeEventInvoked = false;
        bool _windowFocusEventInvoked = false;
        bool _windowIconifyEventInvoked = false;
        bool _windowFramebufferRefreshEventInvoked = false;
        bool _windowFramebufferResizeEventInvoked = false;
        bool _windowApplicationWindowCloseEventInvoked = false;
        bool _windowApplicationWindowMoveEventInvoked = false;
        bool _windowApplicationWindowResizeEventInvoked = false;
        bool _windowApplicationWindowFocusEventInvoked = false;
        bool _windowApplicationWindowIconifyEventInvoked = false;
        bool _windowApplicationWindowFramebufferRefreshEventInvoked = false;
        bool _windowApplicationWindowFramebufferResizeEventInvoked = false;

        bool _duplicateSidFrameListenerCheckActivated = false;
        bool _duplicateSidFrameListenerCheckSuccess = false;
        bool _duplicatePriorityFrameListenerCheckActivated = false;
        bool _duplicatePriorityFrameListenerCheckSuccess = false;
    };
    //--------------------------------------------------------------------------
}