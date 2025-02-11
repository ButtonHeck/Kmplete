#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct WindowEvent : public Event
    {
        KMP_NODISCARD int GetTraits() const noexcept override
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

        WindowResizeEvent(unsigned int width, unsigned int height) noexcept
            : _width(width)
            , _height(height)
        {}

        KMP_NODISCARD unsigned int GetWidth() const noexcept
        {
            return _width;
        }

        KMP_NODISCARD unsigned int GetHeight() const noexcept
        {
            return _height;
        }

        KMP_NODISCARD std::string ToString() const override
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

        WindowMoveEvent(unsigned int x, unsigned int y) noexcept
            : _x(x)
            , _y(y)
        {}

        KMP_NODISCARD unsigned int GetX() const noexcept
        {
            return _x;
        }

        KMP_NODISCARD unsigned int GetY() const noexcept
        {
            return _y;
        }

        KMP_NODISCARD std::string ToString() const override
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

        explicit WindowFocusEvent(int focused) noexcept
            : _focused(focused)
        {}

        KMP_NODISCARD bool IsFocused() const noexcept
        {
            return bool(_focused);
        }

        KMP_NODISCARD std::string ToString() const override
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

        explicit WindowIconifyEvent(int iconified) noexcept
            : _iconified(iconified)
        {}

        KMP_NODISCARD bool IsIconified() const noexcept
        {
            return bool(_iconified);
        }

        KMP_NODISCARD std::string ToString() const override
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

        WindowFramebufferResizeEvent(unsigned int width, unsigned int height) noexcept
            : _width(width)
            , _height(height)
        {}

        KMP_NODISCARD unsigned int GetWidth() const noexcept
        {
            return _width;
        }

        KMP_NODISCARD unsigned int GetHeight() const noexcept
        {
            return _height;
        }

        KMP_NODISCARD std::string ToString() const override
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
