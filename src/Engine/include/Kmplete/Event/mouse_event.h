#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Event/mouse_codes.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct MouseMoveEvent : public Event
    {
        EVENT_CLASS_TYPE(MouseMoveEventType)

        KMP_NODISCARD int GetTraits() const KMP_NOEXCEPT override
        {
            return MouseEventTrait | InputEventTrait;
        }

        MouseMoveEvent(float x, float y) KMP_NOEXCEPT
            : _x(x)
            , _y(y)
        {}

        KMP_NODISCARD float GetX() const KMP_NOEXCEPT
        {
            return _x;
        }

        KMP_NODISCARD float GetY() const KMP_NOEXCEPT
        {
            return _y;
        }

        KMP_NODISCARD std::string ToString() const override
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
        EVENT_CLASS_TYPE(MouseScrollEventType)

        KMP_NODISCARD int GetTraits() const KMP_NOEXCEPT override
        {
            return MouseEventTrait | InputEventTrait;
        }

        MouseScrollEvent(float xOffset, float yOffset) KMP_NOEXCEPT
            : _xOffset(xOffset)
            , _yOffset(yOffset)
        {}

        KMP_NODISCARD float GetXOffset() const KMP_NOEXCEPT
        {
            return _xOffset;
        }

        KMP_NODISCARD float GetYOffset() const KMP_NOEXCEPT
        {
            return _yOffset;
        }

        KMP_NODISCARD std::string ToString() const override
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
        KMP_NODISCARD int GetTraits() const KMP_NOEXCEPT override
        {
            return MouseEventTrait | InputEventTrait | MouseButtonEventTrait;
        }

        KMP_NODISCARD MouseCode GetMouseButton() const KMP_NOEXCEPT
        {
            return _button;
        }

        KMP_NODISCARD int GetMods() const KMP_NOEXCEPT
        {
            return _mods;
        }

    protected:
        MouseButtonEvent(const MouseCode button, int mods) KMP_NOEXCEPT
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
        EVENT_CLASS_TYPE(MouseButtonPressEventType)

        MouseButtonPressEvent(const MouseCode button, int mods) KMP_NOEXCEPT
            : MouseButtonEvent(button, mods)
        {}

        KMP_NODISCARD std::string ToString() const override
        {
            return Utils::Concatenate("MouseButtonPressEvent: ", _button);
        }
    };
    //--------------------------------------------------------------------------


    struct MouseButtonReleaseEvent : public MouseButtonEvent
    {
        EVENT_CLASS_TYPE(MouseButtonReleaseEventType)

        MouseButtonReleaseEvent(const MouseCode button, int mods) KMP_NOEXCEPT
            : MouseButtonEvent(button, mods)
        {}

        KMP_NODISCARD std::string ToString() const override
        {
            return Utils::Concatenate("MouseButtonReleaseEvent: ", _button);
        }
    };
    //--------------------------------------------------------------------------
}
