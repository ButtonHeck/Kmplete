#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Event/mouse_codes.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct MouseMoveEvent : public Event
    {
        EVENT_CLASS_TYPE(MouseMoveEventType)

        int GetTraits() const override
        {
            return MouseEventTrait | InputEventTrait;
        }

        MouseMoveEvent(float x, float y)
            : _x(x)
            , _y(y)
        {}

        float GetX() const
        {
            return _x;
        }

        float GetY() const
        {
            return _y;
        }

        std::string ToString() const override
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

        int GetTraits() const override
        {
            return MouseEventTrait | InputEventTrait;
        }

        MouseScrollEvent(float xOffset, float yOffset)
            : _xOffset(xOffset)
            , _yOffset(yOffset)
        {}

        float GetXOffset() const
        {
            return _xOffset;
        }

        float GetYOffset() const
        {
            return _yOffset;
        }

        std::string ToString() const override
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
        int GetTraits() const override
        {
            return MouseEventTrait | InputEventTrait | MouseButtonEventTrait;
        }

        MouseCode GetMouseButton() const
        {
            return _button;
        }

        int GetMods() const
        {
            return _mods;
        }

    protected:
        MouseButtonEvent(const MouseCode button, int mods)
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

        MouseButtonPressEvent(const MouseCode button, int mods)
            : MouseButtonEvent(button, mods)
        {}

        std::string ToString() const override
        {
            return Utils::Concatenate("MouseButtonPressEvent: ", _button);
        }
    };
    //--------------------------------------------------------------------------


    struct MouseButtonReleaseEvent : public MouseButtonEvent
    {
        EVENT_CLASS_TYPE(MouseButtonReleaseEventType)

        MouseButtonReleaseEvent(const MouseCode button, int mods)
            : MouseButtonEvent(button, mods)
        {}

        std::string ToString() const override
        {
            return Utils::Concatenate("MouseButtonReleaseEvent: ", _button);
        }
    };
    //--------------------------------------------------------------------------
}
