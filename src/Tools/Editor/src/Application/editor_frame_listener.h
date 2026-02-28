#pragma once

#include "UI/editor_ui_compositor.h"
#include "Application/events.h"

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Time/timer.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;

    namespace Graphics
    {
        class GraphicsBackend;
    }

    namespace Assets
    {
        class AssetsManager;
    }

    namespace Input
    {
        class InputManager;
    }


    class EditorFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(EditorFrameListener)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(EditorFrameListener)

    public:
        EditorFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager,
                            LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager);
        ~EditorFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        void _Initialize(LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager);
        void _InitializeImGui(float dpiScale);
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

        KMP_NODISCARD bool _OnWindowCloseEvent(Events::WindowCloseEvent& event);
        KMP_NODISCARD bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);
        KMP_NODISCARD bool _OnEditorFullscreenEvent(Events::EditorFullscreenEvent& event);

        void _AddImGuiFonts(float scale);

    private:
        SystemMetricsManager& _systemMetricsManager;
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        UPtr<EditorUICompositor> _uiCompositor;
        Time::Timer _metricsTimer;
        HashMap<StringID, void*> _imguiTextureIDs;

        Events::EventHandlerGuard<Events::WindowCloseEvent> _windowCloseHandler;
        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
        Events::EventHandlerGuard<Events::EditorFullscreenEvent> _editorFullscreenHandler;
    };
    //--------------------------------------------------------------------------
}