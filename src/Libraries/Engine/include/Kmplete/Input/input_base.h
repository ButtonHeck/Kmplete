#pragma once

#include "Kmplete/Base/functional.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Input/input_codes.h"
#include "Kmplete/Math/geometry.h"

#include <variant>


namespace Kmplete
{
    namespace Input
    {
        //! Alias for a value that input control may have, e.g. a button may either be pressed or released,
        //! joystick has analogue values represented by float, mouse movement have a pair of coordinates/delta.
        //! Special "monostate" has semantics of an "empty" value
        using InputControlValue = std::variant<int, float, Math::Point2I, std::monostate>;

        static constexpr auto ButtonPressed = 1;
        static constexpr auto ButtonReleased = 0;

        static constexpr auto ButtonPressedValue = InputControlValue(ButtonPressed);
        static constexpr auto ButtonReleasedValue = InputControlValue(ButtonReleased);
        static constexpr auto EmptyValue = InputControlValue(std::monostate());

        static constexpr auto InputControlValueIntIndex = 0;
        static constexpr auto InputControlValueFloatIndex = 1;
        static constexpr auto InputControlValuePointIndex = 2;


        //! Alias for human-readable "name" of an action (e.g. "shoot") that connected
        //! to a number of input controls on one side and to the number of callbacks on the other side
        using ActionIdentifier = StringID;

        //! Alias for an action callback tag (or identifier) used to distinguish callbacks that tied to a single action.
        //! If it is not necessary to assign tag to a callback (e.g. single action = single callback) default tag with id 0 is used 
        using ActionCallbackTag = StringID;

        //! Alias for an action callback function signature
        using ActionCallback = Function<bool(InputControlValue)>;


        //! An ActionCallback with its tag number
        struct TaggedActionCallback
        {
            ActionCallbackTag tag;
            ActionCallback callback;
        };
        //--------------------------------------------------------------------------

        static constexpr ActionCallbackTag DefaultActionCallbackTag = 0ULL;


        //! Action-value connection binding. Not a real "event" in a sense that is not used
        //! outside of InputManager
        //! @see Input::InputManager
        struct ActionEvent
        {
            ActionIdentifier id;
            InputControlValue value;
        };
        //--------------------------------------------------------------------------


        //! Helper struct containing rules that dictate whether or not an action event should occur
        //! for an action-value binding
        struct InputCondition
        {
            InputControlValue value = EmptyValue;
            KeyModifier modifierMask = Input::Modifier::None;
            float timerTriggerMs = 0.0f;
        };
        //--------------------------------------------------------------------------

        static constexpr InputCondition PressNoModsCondition = InputCondition{ .value = ButtonPressedValue, .modifierMask = Input::Modifier::None };
        static constexpr InputCondition NoCondition = InputCondition{};


        //! Base binding unit for an action identifiers. Make sure to match code source (digital, analogue, coords)
        //! with the condition's value variant
        struct InputCodeWithCondition
        {
            InputCode code;
            InputCondition condition;
        };
        //--------------------------------------------------------------------------


        //! Unit of the time-based trigger condition, units are milliseconds
        struct TimeCondition
        {
            bool active = false;
            float currentMs = 0.0f;
            float triggerMs = 0.0f;

            inline void Activate()
            {
                active = true;
                currentMs = 0.0f;
            }

            inline void Deactivate()
            {
                active = false;
                currentMs = 0.0f;
            }
        };
        //--------------------------------------------------------------------------
    }
}