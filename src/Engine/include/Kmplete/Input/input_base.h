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
        using InputControlValue = std::variant<int, float, Math::Point2I, std::monostate>;

        static constexpr auto ButtonPressed = 1;
        static constexpr auto ButtonReleased = 0;

        static constexpr auto ButtonPressedValue = InputControlValue(ButtonPressed);
        static constexpr auto ButtonReleasedValue = InputControlValue(ButtonReleased);
        static constexpr auto EmptyValue = InputControlValue(std::monostate());

        static constexpr auto InputControlValueIntIndex = 0;
        static constexpr auto InputControlValueFloatIndex = 1;
        static constexpr auto InputControlValuePointIndex = 2;


        using ActionIdentifier = StringID;
        using ActionCallbackTag = StringID;
        using ActionCallback = Function<bool(InputControlValue)>;


        struct TaggedActionCallback
        {
            ActionCallbackTag tag;
            ActionCallback callback;
        };
        //--------------------------------------------------------------------------

        static constexpr ActionCallbackTag DefaultActionCallbackTag = 0ULL;


        struct ActionEvent
        {
            ActionIdentifier id;
            InputControlValue value;
        };
        //--------------------------------------------------------------------------


        struct InputCondition
        {
            InputControlValue value = EmptyValue;
            KeyModifier modifierMask = Input::Modifier::None;
            float timerTriggerMs = 0.0f;
        };
        //--------------------------------------------------------------------------

        static constexpr InputCondition PressNoModsCondition = InputCondition{ .value = ButtonPressedValue, .modifierMask = Input::Modifier::None };
        static constexpr InputCondition NoCondition = InputCondition{};


        struct InputCodeWithCondition
        {
            InputCode code;
            InputCondition condition;
        };
        //--------------------------------------------------------------------------


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