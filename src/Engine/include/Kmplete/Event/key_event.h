#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_codes.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct KeyEvent : public Event
    {
        int GetTraits() const override
        {
            return KeyboardEventTrait | InputEventTrait;
        }

        KeyCode GetKeyCode() const
        {
            return _keyCode;
        }

    protected:
        explicit KeyEvent(const KeyCode keyCode)
            : _keyCode(keyCode)
        {}

    protected:
        const KeyCode _keyCode;
    };
    //--------------------------------------------------------------------------


    struct KeyPressEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyPressEventType)

        KeyPressEvent(const KeyCode keyCode, int mods, bool repeat = false)
            : KeyEvent(keyCode)
            , _mods(mods)
            , _repeat(repeat)
        {}

        bool IsRepeat() const
        {
            return _repeat;
        }

        int GetMods() const
        {
            return _mods;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyPressEvent: ", _keyCode, " (repeat = ", _repeat, ", mods = ", _mods, ")");
        }

    private:
        const int _mods;
        const bool _repeat;
    };
    //--------------------------------------------------------------------------


    struct KeyReleaseEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyReleaseEventType)

        explicit KeyReleaseEvent(const KeyCode keyCode)
            : KeyEvent(keyCode)
        {}

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyReleaseEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------


    struct KeyCharEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyCharEventType)

        explicit KeyCharEvent(const KeyCode keyCode)
            : KeyEvent(keyCode)
        {}

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyCharEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------
}
