#pragma once

#include "editor_ui_impl.h"
#include "editor_ui_compositor.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"

namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;
    class GraphicsBackend;

    class EditorUI
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUI)

        EditorUI(Window& mainWindow, float dpiScale, GraphicsBackend& graphicsBackend, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager);
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
        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event);

        void SaveSettings(Settings& settings) const;
        void LoadSettings(Settings& settings);

    private:
        void Initialize(Window& mainWindow, GraphicsBackendType graphicsBackendType);
        void AddDefaultFont() const;
        void AddIconsFont() const;
        void Stylize() const;
        void Finalize() const;

    private:
        SystemMetricsManager& _systemMetricsManager;
        float _dpiScale;
        UPtr<EditorUIImpl> _uiImpl;
        UPtr<EditorUICompositor> _compositor;
        Timer _metricsTimer;
    };
    //--------------------------------------------------------------------------
}