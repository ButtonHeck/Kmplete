#pragma once

#include "Kmplete/Application/application.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Core/timer.h"


namespace Kmplete
{
    class WindowApplication : public Application
    {
        KMP_LOG_CLASSNAME(WindowApplication)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(WindowApplication)

        KMP_API explicit WindowApplication(const ApplicationParameters& applicationParameters);
        KMP_API virtual ~WindowApplication();

        KMP_API void Run() override;
        KMP_NODISCARD KMP_API virtual bool ConfirmExit();

    protected:
        void OnEvent(Event& event);

    private:
        void Initialize();
        void Finalize();

        void UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified);
        void RenderFrameListeners();
        void ProcessEventsFrameListeners(Event& event);

        void SaveSettings() const;
        void LoadSettings();

    protected:
        UPtr<WindowBackend> _windowBackend;
        GraphicsBackendType _graphicsBackendType;
        UPtr<GraphicsBackend> _graphicsBackend;

    private:
        Timer _frameTimer;
    };
    //--------------------------------------------------------------------------
}