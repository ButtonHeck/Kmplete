#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/application.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/mouse_event.h"

namespace Kmplete
{
    class KMP_API WindowApplication : public Application
    {
    public:
        WindowApplication();

        KMP_NODISCARD bool Initialize(const std::string& settingsFilePath) override;

    protected:
        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event) { return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event) { return true; }
    };
    //--------------------------------------------------------------------------
}