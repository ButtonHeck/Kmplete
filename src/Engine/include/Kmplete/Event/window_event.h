#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Utils/string_utils.h"


namespace Kmplete
{
    namespace Events
    {
        static constexpr auto WindowResizeEventTypeStr = "WindowResizeEvent";
        static constexpr auto WindowMoveEventTypeStr = "WindowMoveEvent";
        static constexpr auto WindowCloseEventTypeStr = "WindowCloseEvent";
        static constexpr auto WindowFocusEventTypeStr = "WindowFocusEvent";
        static constexpr auto WindowIconifyEventTypeStr = "WindowIconifyEvent";
        static constexpr auto WindowFramebufferResizeEventTypeStr = "WindowFramebufferResizeEvent";
        static constexpr auto WindowFramebufferRefreshEventTypeStr = "WindowFramebufferRefreshEvent";
        static constexpr auto WindowContentScaleEventTypeStr = "WindowContentScaleEvent";

        static constexpr auto WindowResizeEventTypeID = "WindowResizeEvent"_sid;
        static constexpr auto WindowMoveEventTypeID = "WindowMoveEvent"_sid;
        static constexpr auto WindowCloseEventTypeID = "WindowCloseEvent"_sid;
        static constexpr auto WindowFocusEventTypeID = "WindowFocusEvent"_sid;
        static constexpr auto WindowIconifyEventTypeID = "WindowIconifyEvent"_sid;
        static constexpr auto WindowFramebufferResizeEventTypeID = "WindowFramebufferResizeEvent"_sid;
        static constexpr auto WindowFramebufferRefreshEventTypeID = "WindowFramebufferRefreshEvent"_sid;
        static constexpr auto WindowContentScaleEventTypeID = "WindowContentScaleEvent"_sid;


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
            EVENT_CLASS_TYPE(WindowResizeEventTypeStr)

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

            KMP_NODISCARD String ToString() const override
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
            EVENT_CLASS_TYPE(WindowMoveEventTypeStr)

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

            KMP_NODISCARD String ToString() const override
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
            EVENT_CLASS_TYPE(WindowCloseEventTypeStr)

            WindowCloseEvent() = default;
        };
        //--------------------------------------------------------------------------


        struct WindowFocusEvent : public WindowEvent
        {
            EVENT_CLASS_TYPE(WindowFocusEventTypeStr)

            explicit WindowFocusEvent(int focused) noexcept
                : _focused(focused)
            {}

            KMP_NODISCARD bool IsFocused() const noexcept
            {
                return bool(_focused);
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("WindowFocusEvent: ", IsFocused());
            }

        private:
            const int _focused;
        };
        //--------------------------------------------------------------------------


        struct WindowIconifyEvent : public WindowEvent
        {
            EVENT_CLASS_TYPE(WindowIconifyEventTypeStr)

            explicit WindowIconifyEvent(int iconified) noexcept
                : _iconified(iconified)
            {}

            KMP_NODISCARD bool IsIconified() const noexcept
            {
                return bool(_iconified);
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("WindowIconifyEvent: ", IsIconified());
            }

        private:
            const int _iconified;
        };
        //--------------------------------------------------------------------------


        struct WindowFramebufferResizeEvent : public WindowEvent
        {
            EVENT_CLASS_TYPE(WindowFramebufferResizeEventTypeStr)

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

            KMP_NODISCARD String ToString() const override
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
            EVENT_CLASS_TYPE(WindowFramebufferRefreshEventTypeStr)

            WindowFramebufferRefreshEvent() = default;
        };
        //--------------------------------------------------------------------------


        struct WindowContentScaleEvent : public WindowEvent
        {
            EVENT_CLASS_TYPE(WindowContentScaleEventTypeStr)

            WindowContentScaleEvent(float scale) noexcept
                : _scale(scale)
            {}

            KMP_NODISCARD float GetScale() const noexcept
            {
                return _scale;
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("WindowContentScaleEvent: ", _scale);
            }

        private:
            const float _scale;
        };
        //--------------------------------------------------------------------------
    }
}
