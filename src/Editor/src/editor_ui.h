#pragma once

#include "editor_ui_impl.h"
#include "editor_ui_compositor.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;
    class GraphicsBackend;

    class EditorUI
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(EditorUI)

        EditorUI(Window& mainWindow, GraphicsBackend& graphicsBackend, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager);
        ~EditorUI();

        void LoopIteration();

        void NewFrame();
        void BeginApplicationArea() const;
        void BeginMainWorkingArea() const;
        void ComposeMainArea();
        void EndMainWorkingArea() const;
        void BeginStatusBarArea() const;
        void ComposeStatusBar();
        void EndStatusBarArea() const;
        void EndApplicationArea() const;
        void Render();
        void EndFrame() const;

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event);
        KMP_NODISCARD bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event);
        KMP_NODISCARD bool OnWindowContentScaleEvent(WindowContentScaleEvent& event);
        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event);

        void SaveSettings(Settings& settings) const;
        void LoadSettings(Settings& settings);

    private:
        void Initialize();
        void AddDefaultFont(float dpiScale) const;
        void AddIconsFont(float dpiScale) const;
        void Stylize(float dpiScale) const;
        void Finalize() const;

    private:
        SystemMetricsManager& _systemMetricsManager;
        Window& _mainWindow;
        GraphicsBackend& _graphicsBackend;
        UPtr<EditorUIImpl> _uiImpl;
        UPtr<EditorUICompositor> _compositor;
        Timer _metricsTimer;
    };
    //--------------------------------------------------------------------------
}