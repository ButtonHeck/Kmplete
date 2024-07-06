#pragma once

#include "kmplete_export.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/application.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/mouse_event.h"

namespace Kmplete
{
    class KMPLETE_API WindowApplication : public Application
    {
    public:
        WindowApplication();

        bool Initialize(const std::string& configPath) override;

    protected:
        virtual bool OnWindowMoveEvent(WindowMoveEvent& event) { return true; }
        virtual bool OnWindowCloseEvent(WindowCloseEvent& event) { return true; }
        virtual bool OnWindowResizeEvent(WindowResizeEvent& event) { return true; }
        virtual bool OnWindowFocusEvent(WindowFocusEvent& event) { return true; }
        virtual bool OnWindowIconifyEvent(WindowIconifyEvent& event) { return true; }
        virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) { return true; }
        virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event) { return true; }

        virtual bool OnMouseMoveEvent(MouseMoveEvent& event) { return true; }
        virtual bool OnMouseScrollEvent(MouseScrollEvent& event) { return true; }
        virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& event) { return true; }
        virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event) { return true; }
    };
    //--------------------------------------------------------------------------
}