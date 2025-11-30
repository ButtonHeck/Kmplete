#pragma once

#include "UI/editor_ui_compositor.h"

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;
    class GraphicsBackend;

    namespace Assets
    {
        class AssetsManager;
    }


    class EditorFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(EditorFrameListener)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(EditorFrameListener)

        EditorFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager);
        ~EditorFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified);
        void Render() override;

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        void _Initialize();
        void _Finalize();

        void _NewFrame();
        void _BeginApplicationArea() const;
        void _BeginMainWorkingArea() const;
        void _ComposeMainArea();
        void _EndMainWorkingArea() const;
        void _BeginStatusBarArea() const;
        void _ComposeStatusBar();
        void _EndStatusBarArea() const;
        void _EndApplicationArea() const;
        void _EndFrame() const;

        KMP_NODISCARD bool _OnWindowCloseEvent(WindowCloseEvent& event);
        KMP_NODISCARD bool _OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event);
        KMP_NODISCARD bool _OnWindowContentScaleEvent(WindowContentScaleEvent& event);
        KMP_NODISCARD bool _OnKeyPressEvent(KeyPressEvent& event);

        void _AddImGuiFonts(float scale);

    private:
        SystemMetricsManager& _systemMetricsManager;
        Window& _mainWindow;
        GraphicsBackend& _graphicsBackend;
        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        UPtr<EditorUICompositor> _uiCompositor;
        Timer _metricsTimer;

        EventHandlerGuard<WindowCloseEvent> _windowCloseHandler;
        EventHandlerGuard<WindowFramebufferRefreshEvent> _windowFramebufferRefreshHandler;
        EventHandlerGuard<WindowContentScaleEvent> _windowContentScaleHandler;
        EventHandlerGuard<KeyPressEvent> _keyPressHandler;
    };
    //--------------------------------------------------------------------------
}