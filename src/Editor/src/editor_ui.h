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

    class EditorUI
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUI)

        EditorUI(const Ptr<Window> window, float dpiScale, const Ptr<LocalizationManager> localizationManager, const Ptr<SystemMetricsManager> systemMetricsManager);
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
        void Initialize(const Ptr<Window> window, float dpiScale);
        void AddDefaultFont(float dpiScale) const;
        void Stylize(float dpiScale) const;
        void Finalize() const;

    private:
        UPtr<EditorUIImpl> _uiImpl;
        const Ptr<SystemMetricsManager> _systemMetricsManager;
        const UPtr<EditorUICompositor> _compositor;
        Timer _metricsTimer;
    };
    //--------------------------------------------------------------------------
}