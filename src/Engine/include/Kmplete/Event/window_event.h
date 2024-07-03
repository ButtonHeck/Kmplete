#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct WindowEvent : public Event
    {
        int GetTraits() const override
        {
            return WindowEventTrait;
        }

    protected:
        WindowEvent() = default;
    };
    //--------------------------------------------------------------------------


    struct WindowResizeEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowResizeEventType)

        WindowResizeEvent(unsigned int width, unsigned int height)
            : _width(width)
            , _height(height)
        {}

        unsigned int GetWidth() const
        {
            return _width;
        }

        unsigned int GetHeight() const
        {
            return _height;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowResizeEvent: ", _width, ", ", _height);
        }

    private:
        const unsigned int _width;
        const unsigned int _height;
    };
    //--------------------------------------------------------------------------


    struct WindowMoveEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowMoveEventType)

        WindowMoveEvent(unsigned int x, unsigned int y)
            : _x(x)
            , _y(y)
        {}

        unsigned int GetX() const
        {
            return _x;
        }

        unsigned int GetY() const
        {
            return _y;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowMoveEvent: ", _x, ", ", _y);
        }

    private:
        const unsigned int _x;
        const unsigned int _y;
    };
    //--------------------------------------------------------------------------


    struct WindowCloseEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowCloseEventType)

        WindowCloseEvent() = default;
    };
    //--------------------------------------------------------------------------


    struct WindowFocusEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowFocusEventType)

        explicit WindowFocusEvent(int focused)
            : _focused(focused)
        {}

        bool IsFocused() const
        {
            return bool(_focused);
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowFocusEvent: ", IsFocused());
        }

    private:
        const int _focused;
    };
    //--------------------------------------------------------------------------


    struct WindowIconifyEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowIconifyEventType)

        explicit WindowIconifyEvent(int iconified)
            : _iconified(iconified)
        {}

        bool IsIconified() const
        {
            return bool(_iconified);
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowIconifyEvent: ", IsIconified());
        }

    private:
        const int _iconified;
    };
    //--------------------------------------------------------------------------


    struct WindowFramebufferResizeEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowFramebufferResizeEventType)

        WindowFramebufferResizeEvent(unsigned int width, unsigned int height)
            : _width(width)
            , _height(height)
        {}

        unsigned int GetWidth() const
        {
            return _width;
        }

        unsigned int GetHeight() const
        {
            return _height;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowFramebufferResizeEvent: ", _width, ", ", _height);
        }

    private:
        const unsigned int _width;
        const unsigned int _height;
    };
    //--------------------------------------------------------------------------


    struct WindowFramebufferRefreshEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowFramebufferRefreshEventType)

        WindowFramebufferRefreshEvent() = default;
    };
    //--------------------------------------------------------------------------
}
