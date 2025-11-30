#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Event/mouse_codes.h"
#include "Kmplete/Utils/string_utils.h"


namespace Kmplete
{
    namespace Events
    {
        static constexpr auto MouseMoveEventTypeStr = "MouseMoveEvent";
        static constexpr auto MouseScrollEventTypeStr = "MouseScrollEvent";
        static constexpr auto MouseButtonPressEventTypeStr = "MouseButtonPressEvent";
        static constexpr auto MouseButtonReleaseEventTypeStr = "MouseButtonReleaseEvent";

        static constexpr auto MouseMoveEventTypeID = "MouseMoveEvent"_sid;
        static constexpr auto MouseScrollEventTypeID = "MouseScrollEvent"_sid;
        static constexpr auto MouseButtonPressEventTypeID = "MouseButtonPressEvent"_sid;
        static constexpr auto MouseButtonReleaseEventTypeID = "MouseButtonReleaseEvent"_sid;


        struct MouseMoveEvent : public Event
        {
            EVENT_CLASS_TYPE(MouseMoveEventTypeStr)

            KMP_NODISCARD int GetTraits() const noexcept override
            {
                return MouseEventTrait | InputEventTrait;
            }

            MouseMoveEvent(float x, float y) noexcept
                : _x(x)
                , _y(y)
            {}

            KMP_NODISCARD float GetX() const noexcept
            {
                return _x;
            }

            KMP_NODISCARD float GetY() const noexcept
            {
                return _y;
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("MouseMoveEvent: ", _x, ", ", _y);
            }

        private:
            const float _x;
            const float _y;
        };
        //--------------------------------------------------------------------------


        struct MouseScrollEvent : public Event
        {
            EVENT_CLASS_TYPE(MouseScrollEventTypeStr)

            KMP_NODISCARD int GetTraits() const noexcept override
            {
                return MouseEventTrait | InputEventTrait;
            }

            MouseScrollEvent(float xOffset, float yOffset) noexcept
                : _xOffset(xOffset)
                , _yOffset(yOffset)
            {}

            KMP_NODISCARD float GetXOffset() const noexcept
            {
                return _xOffset;
            }

            KMP_NODISCARD float GetYOffset() const noexcept
            {
                return _yOffset;
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("MouseScrollEvent: ", _xOffset, ", ", _yOffset);
            }

        private:
            const float _xOffset;
            const float _yOffset;
        };
        //--------------------------------------------------------------------------


        struct MouseButtonEvent : public Event
        {
            KMP_NODISCARD int GetTraits() const noexcept override
            {
                return MouseEventTrait | InputEventTrait | MouseButtonEventTrait;
            }

            KMP_NODISCARD MouseCode GetMouseButton() const noexcept
            {
                return _button;
            }

            KMP_NODISCARD int GetMods() const noexcept
            {
                return _mods;
            }

        protected:
            MouseButtonEvent(const MouseCode button, int mods) noexcept
                : _button(button)
                , _mods(mods)
            {}

        protected:
            const MouseCode _button;
            const int _mods;
        };
        //--------------------------------------------------------------------------


        struct MouseButtonPressEvent : public MouseButtonEvent
        {
            EVENT_CLASS_TYPE(MouseButtonPressEventTypeStr)

            MouseButtonPressEvent(const MouseCode button, int mods) noexcept
                : MouseButtonEvent(button, mods)
            {}

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("MouseButtonPressEvent: ", _button);
            }
        };
        //--------------------------------------------------------------------------


        struct MouseButtonReleaseEvent : public MouseButtonEvent
        {
            EVENT_CLASS_TYPE(MouseButtonReleaseEventTypeStr)

            MouseButtonReleaseEvent(const MouseCode button, int mods) noexcept
                : MouseButtonEvent(button, mods)
            {}

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("MouseButtonReleaseEvent: ", _button);
            }
        };
        //--------------------------------------------------------------------------
    }
}
