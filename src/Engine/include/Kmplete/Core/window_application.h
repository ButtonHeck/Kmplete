#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/application.h"
#include "Kmplete/Core/window_backend.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/mouse_event.h"

namespace Kmplete
{
    class WindowApplication : public Application
    {
    public:
        KMP_API WindowApplication();

        KMP_API KMP_NODISCARD bool Initialize(const std::string& settingsFilePath) override;
        KMP_API void Finalize() override;

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
        void SaveSettings() const;
        void LoadSettings();

    protected:
        Ptr<WindowBackend> _backend;
        Ptr<Window> _mainWindow;
    };
    //--------------------------------------------------------------------------
}