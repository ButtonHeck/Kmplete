#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_codes.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    struct KeyEvent : public Event
    {
        KMP_NODISCARD int GetTraits() const KMP_NOEXCEPT override
        {
            return KeyboardEventTrait | InputEventTrait;
        }

        KMP_NODISCARD KeyCode GetKeyCode() const KMP_NOEXCEPT
        {
            return _keyCode;
        }

    protected:
        explicit KeyEvent(const KeyCode keyCode) KMP_NOEXCEPT
            : _keyCode(keyCode)
        {}

    protected:
        const KeyCode _keyCode;
    };
    //--------------------------------------------------------------------------


    struct KeyPressEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyPressEventType)

        KeyPressEvent(const KeyCode keyCode, int mods, bool repeat = false) KMP_NOEXCEPT
            : KeyEvent(keyCode)
            , _mods(mods)
            , _repeat(repeat)
        {}

        KMP_NODISCARD bool IsRepeat() const KMP_NOEXCEPT
        {
            return _repeat;
        }

        KMP_NODISCARD int GetMods() const KMP_NOEXCEPT
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

        explicit KeyReleaseEvent(const KeyCode keyCode) KMP_NOEXCEPT
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

        explicit KeyCharEvent(const KeyCode keyCode) KMP_NOEXCEPT
            : KeyEvent(keyCode)
        {}

        KMP_NODISCARD std::string ToString() const override
        {
            return Utils::Concatenate("KeyCharEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------
}
