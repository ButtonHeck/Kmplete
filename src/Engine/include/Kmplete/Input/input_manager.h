#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Input/input_codes.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler.h"

#include <variant>


namespace Kmplete
{
    namespace Events
    {
        struct Event;
    }


    namespace Input
    {
        using InputControlValue = std::variant<int, float, Math::Point2I>;

        static constexpr InputControlValue ButtonPressed = InputControlValue{ 1 };
        static constexpr InputControlValue ButtonReleased = InputControlValue{ 0 };

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


        class InputManager
        {
            KMP_LOG_CLASSNAME(InputManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_DISABLE_COPY_MOVE(InputManager)

            KMP_API InputManager() noexcept;

            KMP_API void ProcessInputEvents(Events::Event& event);
            KMP_API void PropagateActionEvents();

            KMP_API bool MapActionToCallback(ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapActionToCallback(ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);
            KMP_API bool UnmapActionFromCallback(ActionIdentifier actionId, const ActionCallbackTag& callbackTag);

            KMP_API bool MapInputToAction(InputCode code, ActionIdentifier actionId);
            KMP_API bool UnmapInputFromAction(InputCode code, ActionIdentifier actionId);
            KMP_API bool RemapInputToAction(InputCode code, ActionIdentifier actionId);

            KMP_API bool MapInputToCallback(InputCode code, ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapInputToCallback(InputCode code, ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);

            KMP_NODISCARD KMP_API const Math::Point2I& GetMousePosition() const noexcept;
            KMP_NODISCARD KMP_API bool IsMouseButtonPressed(InputCode mouseCode) const;
            KMP_NODISCARD KMP_API KeyModifier GetKeyModifiersMask() const noexcept;
            KMP_NODISCARD KMP_API bool IsKeyButtonPressed(InputCode keyCode) const;

            template<typename ExpectedType> requires (IsAnyOfType<ExpectedType, int, float, Math::Point2I>)
            KMP_NODISCARD InputControlValue GetActionValue(ActionIdentifier actionId)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

                if (!_actionToInputCodesMap.contains(actionId))
                {
                    return InputControlValue(ExpectedType());
                }

                const auto& codes = _actionToInputCodesMap[actionId];
                InputControlValue resultValue = ExpectedType();
                for (const auto& code : codes)
                {
                    if (code < 0 || code >= Code::NumCodes)
                    {
                        continue;
                    }

                    const auto& currentValue = _controlStates[code];
                    const auto currentUnderlyingValue = std::get<ExpectedType>(currentValue);
                    const auto resultUnderlyingValue = std::get<ExpectedType>(resultValue);

                    if (std::abs(currentUnderlyingValue) > std::abs(resultUnderlyingValue))
                    {
                        resultValue = currentValue;
                    }
                }

                return resultValue;
            }

            template<>
            KMP_NODISCARD InputControlValue GetActionValue<Math::Point2I>(ActionIdentifier actionId)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

                if (!_actionToInputCodesMap.contains(actionId))
                {
                    return InputControlValue(Math::Point2I());
                }

                const auto& codes = _actionToInputCodesMap[actionId];
                InputControlValue resultValue = Math::Point2I();
                for (const auto& code : codes)
                {
                    if (code < 0 || code >= Code::NumCodes)
                    {
                        continue;
                    }

                    const auto& currentValue = _controlStates[code];
                    const auto currentUnderlyingValue = std::get<Math::Point2I>(currentValue);

                    if (currentUnderlyingValue != Math::Point2I())
                    {
                        resultValue = currentValue;
                        break;
                    }
                }

                return resultValue;
            }

        private:
            KMP_NODISCARD Vector<ActionEvent> _CreateActionEvents(InputCode code, InputControlValue value) const;
            void _PropagateSingleActionEvent(const ActionEvent& actionEvent);
            KMP_NODISCARD bool _ContainsTaggedCallback(const Vector<TaggedActionCallback>& callbacks, const TaggedActionCallback& taggedCallback) const;
            KMP_NODISCARD bool _ContainsActionIdentifier(const Vector<ActionIdentifier>& actionsIdentifiers, ActionIdentifier actionId) const;

        private:
            Math::Point2I _mousePosition;
            Array<InputControlValue, Code::NumCodes> _controlStates;
            KeyModifier _modifiersMask;

            HashMap<InputCode, Vector<ActionIdentifier>> _inputCodeToActionsMap;
            HashMap<ActionIdentifier, Vector<InputCode>> _actionToInputCodesMap;
            HashMap<ActionIdentifier, Vector<TaggedActionCallback>> _actionCallbacks;

            Vector<ActionEvent> _actionEvents;
        };
        //--------------------------------------------------------------------------
    }
}