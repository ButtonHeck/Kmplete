#pragma once

#include "Kmplete/Application/application.h"
#include "Kmplete/Core/window_backend.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/mouse_event.h"

namespace Kmplete
{
    class WindowApplication : public Application
    {
    public:
        KMP_DISABLE_COPY_MOVE(WindowApplication)

        KMP_API explicit WindowApplication(const ApplicationParameters& applicationParameters);
        KMP_API virtual ~WindowApplication();

        KMP_API void SaveSettings(const Path& path = Path()) const override;
        KMP_API void LoadSettings(const Path& path = Path()) override;

    protected:
        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent&) { return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent&) { return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent&) { return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent&) { return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent&) { return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&) { return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent&) { return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent&) { return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent&) { return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent&) { return true; }
        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent&) { return true; }

    private:
        void Initialize();
        void Finalize();
        void SaveSettingsInternal() const;
        void LoadSettingsInternal();

    protected:
        UPtr<WindowBackend> _backend;
        Ptr<Window> _mainWindow;
    };
    //--------------------------------------------------------------------------
}