#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_codes.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct KeyEvent : public Event
    {
        KMP_NODISCARD int GetTraits() const noexcept override
        {
            return KeyboardEventTrait | InputEventTrait;
        }

        KMP_NODISCARD KeyCode GetKeyCode() const noexcept
        {
            return _keyCode;
        }

    protected:
        explicit KeyEvent(const KeyCode keyCode) noexcept
            : _keyCode(keyCode)
        {}

    protected:
        const KeyCode _keyCode;
    };
    //--------------------------------------------------------------------------


    struct KeyPressEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyPressEventType)

        KeyPressEvent(const KeyCode keyCode, int mods, bool repeat = false) noexcept
            : KeyEvent(keyCode)
            , _mods(mods)
            , _repeat(repeat)
        {}

        KMP_NODISCARD bool IsRepeat() const noexcept
        {
            return _repeat;
        }

        KMP_NODISCARD int GetMods() const noexcept
        {
            return _mods;
        }

        KMP_NODISCARD std::string ToString() const override
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

        explicit KeyReleaseEvent(const KeyCode keyCode) noexcept
            : KeyEvent(keyCode)
        {}

        KMP_NODISCARD std::string ToString() const override
        {
            return Utils::Concatenate("KeyReleaseEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------


    struct KeyCharEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyCharEventType)

        explicit KeyCharEvent(const KeyCode keyCode) noexcept
            : KeyEvent(keyCode)
        {}

        KMP_NODISCARD std::string ToString() const override
        {
            return Utils::Concatenate("KeyCharEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------
}
