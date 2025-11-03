#pragma once

#include "Kmplete/Application/application.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Base/nullability.h"


namespace Kmplete
{
    struct WindowApplicationParameters
    {
        const ApplicationParameters applicationParameters;
        const bool resizable;
    };
    //--------------------------------------------------------------------------

    class WindowApplication : public Application
    {
        KMP_LOG_CLASSNAME(WindowApplication)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(WindowApplication)

        KMP_API explicit WindowApplication(const WindowApplicationParameters& parameters);
        KMP_API virtual ~WindowApplication();

        KMP_API void Run() override;
        KMP_API void AddFrameListener(NonNull<FrameListener*> frameListener);

        KMP_NODISCARD KMP_API virtual bool ConfirmExit();

    protected:
        void OnEvent(Event& event);

    private:
        void Initialize(const WindowApplicationParameters& parameters);
        void Finalize();

        void UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified);
        void RenderFrameListeners();
        void ProcessEventsFrameListeners(Event& event);

        void SaveSettings() const;
        void LoadSettings();

    protected:
        UPtr<WindowBackend> _windowBackend;
        UPtr<GraphicsBackend> _graphicsBackend;
        UPtr<Assets::AssetsManager> _assetsManager;

    private:
        GraphicsBackendType _graphicsBackendType;
        Timer _frameTimer;
        Vector<FrameListenerWrapper> _frameListenersWrappers;
    };
    //--------------------------------------------------------------------------
}