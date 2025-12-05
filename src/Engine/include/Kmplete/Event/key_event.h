#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Input/key_codes.h"
#include "Kmplete/Utils/string_utils.h"


namespace Kmplete
{
    namespace Events
    {
        static constexpr auto KeyPressEventTypeStr = "KeyPressEvent";
        static constexpr auto KeyReleaseEventTypeStr = "KeyReleaseEvent";
        static constexpr auto KeyCharEventTypeStr = "KeyCharEvent";

        static constexpr auto KeyPressEventTypeID = "KeyPressEvent"_sid;
        static constexpr auto KeyReleaseEventTypeID = "KeyReleaseEvent"_sid;
        static constexpr auto KeyCharEventTypeID = "KeyCharEvent"_sid;


        struct KeyEvent : public Event
        {
            KMP_NODISCARD Input::KeyCode GetKeyCode() const noexcept
            {
                return _keyCode;
            }

        protected:
            explicit KeyEvent(const Input::KeyCode keyCode) noexcept
                : _keyCode(keyCode)
            {}

        protected:
            const Input::KeyCode _keyCode;
        };
        //--------------------------------------------------------------------------


        struct KeyPressEvent : public KeyEvent
        {
            EVENT_CLASS_TYPE(KeyPressEventTypeStr)

            KeyPressEvent(const Input::KeyCode keyCode, int mods, bool repeat = false) noexcept
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

            KMP_NODISCARD String ToString() const override
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
            EVENT_CLASS_TYPE(KeyReleaseEventTypeStr)

            explicit KeyReleaseEvent(const Input::KeyCode keyCode) noexcept
                : KeyEvent(keyCode)
            {}

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("KeyReleaseEvent: ", _keyCode);
            }
        };
        //--------------------------------------------------------------------------


        struct KeyCharEvent : public KeyEvent
        {
            EVENT_CLASS_TYPE(KeyCharEventTypeStr)

            explicit KeyCharEvent(const Input::KeyCode keyCode) noexcept
                : KeyEvent(keyCode)
            {}

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("KeyCharEvent: ", _keyCode);
            }
        };
        //--------------------------------------------------------------------------
    }
}
