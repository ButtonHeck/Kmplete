#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Input/input_base.h"
#include "Kmplete/Input/input_codes.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Events
    {
        struct Event;
        struct MouseMoveEvent;
        struct MouseButtonPressEvent;
        struct MouseButtonReleaseEvent;
        struct KeyPressEvent;
        struct KeyReleaseEvent;
    }


    namespace Input
    {
        //! Manager responsible for handling low-level input events, adding/removing bindings between
        //! input combinations (optionally with conditions) and Actions, and bindings between Actions and
        //! callback handlers. InputManager internally contains an event queue for ActionEvent objects
        //! that trigger dedicated callbacks.
        //! Also available for polling user input control states (both for a simple code or an action identifier).
        //! @see input_base.h
        //! @see Events::Events
        class InputManager
        {
            KMP_LOG_CLASSNAME(InputManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(InputManager)

        public:
            KMP_API InputManager() noexcept;

            KMP_API void ProcessInputEvents(Events::Event& event);
            KMP_API void PropagateActionEvents();

            KMP_API bool MapActionToCallback(ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapActionToCallback(ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);
            KMP_API bool UnmapActionFromCallback(ActionIdentifier actionId, const ActionCallbackTag& callbackTag);

            KMP_API bool MapInputToAction(InputCode code, ActionIdentifier actionId);
            KMP_API bool MapInputToAction(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId);
            KMP_API bool UnmapInputFromAction(InputCode code, ActionIdentifier actionId);
            KMP_API bool RemapInputToAction(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId);

            KMP_API bool MapInputToCallback(InputCode code, ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapInputToCallback(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapInputToCallback(InputCode code, ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);
            KMP_API bool MapInputToCallback(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);

            KMP_NODISCARD KMP_API const Math::Point2I& GetMousePosition() const noexcept;
            KMP_NODISCARD KMP_API bool IsMouseButtonPressed(InputCode mouseCode) const;
            KMP_NODISCARD KMP_API KeyModifier GetKeyModifiersMask() const noexcept;
            KMP_NODISCARD KMP_API bool IsKeyButtonPressed(InputCode keyCode) const;

            KMP_API void ResetMouseMove() noexcept;
            KMP_API void UpdateTimerActions(float frameTimestep);

            template<typename ValueType> requires (IsAnyOfType<ValueType, int, float, Math::Point2I>)
            KMP_NODISCARD ValueType GetActionValue(ActionIdentifier actionId) KMP_PROFILING(ProfileLevelImportantVerbose)
            {
                if (!_actionToInputCodesMap.contains(actionId))
                {
                    return ValueType();
                }

                const auto& codes = _actionToInputCodesMap[actionId];
                InputControlValue resultValue = ValueType();
                for (const auto& codeWithCondition : codes)
                {
                    const auto& code = codeWithCondition.code;
                    if (!_IsValidInputCode(code))
                    {
                        continue;
                    }

                    const auto& currentValue = _controlStates[code];
                    const auto& currentUnderlyingValue = std::get<ValueType>(currentValue);
                    const auto& resultUnderlyingValue = std::get<ValueType>(resultValue);

                    if (std::abs(currentUnderlyingValue) > std::abs(resultUnderlyingValue))
                    {
                        resultValue = currentValue;
                    }
                }

                return std::get<ValueType>(resultValue);
            }}
            //--------------------------------------------------------------------------

            template<>
            KMP_NODISCARD Math::Point2I GetActionValue<Math::Point2I>(ActionIdentifier actionId) KMP_PROFILING(ProfileLevelImportantVerbose)
            {
                if (!_actionToInputCodesMap.contains(actionId))
                {
                    return Math::Point2I();
                }

                const auto& codes = _actionToInputCodesMap[actionId];
                InputControlValue resultValue = Math::Point2I();
                for (const auto& codeWithCondition : codes)
                {
                    const auto code = codeWithCondition.code;
                    if (!_IsValidInputCode(code))
                    {
                        continue;
                    }

                    const auto& currentValue = _controlStates[code];
                    const auto& currentUnderlyingValue = std::get<Math::Point2I>(currentValue);

                    if (currentUnderlyingValue != Math::Point2I())
                    {
                        resultValue = currentValue;
                        break;
                    }
                }

                return std::get<Math::Point2I>(resultValue);
            }}
            //--------------------------------------------------------------------------

        private:
            void _ProcessMouseMoveEvent(const Events::MouseMoveEvent& mouseMoveEvent);
            void _ProcessMouseButtonPressEvent(const Events::MouseButtonPressEvent& mouseButtonPressEvent);
            void _ProcessMouseButtonReleaseEvent(const Events::MouseButtonReleaseEvent& mouseButtonReleaseEvent);
            void _ProcessKeyPressEvent(const Events::KeyPressEvent& keyPressEvent);
            void _ProcessKeyReleaseEvent(const Events::KeyReleaseEvent& keyReleaseEvent);

            void _UpdateActionEvents(InputCode code, InputControlValue value, bool isActivation);
            KMP_NODISCARD Vector<ActionEvent> _CreateActionEvents(InputCode code, InputControlValue value) const;
            void _PropagateSingleActionEvent(const ActionEvent& actionEvent);
            KMP_NODISCARD bool _ContainsTaggedCallback(const Vector<TaggedActionCallback>& callbacks, const TaggedActionCallback& taggedCallback) const;
            KMP_NODISCARD bool _ContainsActionIdentifier(const Vector<ActionIdentifier>& actionsIdentifiers, ActionIdentifier actionId) const;

            KMP_NODISCARD KMP_API bool _IsValidMouseButtonCode(InputCode code) const noexcept;
            KMP_NODISCARD KMP_API bool _IsValidKeyboardCode(InputCode code) const noexcept;
            KMP_NODISCARD KMP_API bool _IsValidInputCode(InputCode code) const noexcept;

        private:
            Math::Point2I _mousePosition;
            Array<InputControlValue, Code::NumCodes> _controlStates;
            KeyModifier _modifiersMask;

            HashMap<InputCode, Vector<ActionIdentifier>> _inputCodeToActionsMap;
            HashMap<ActionIdentifier, Vector<InputCodeWithCondition>> _actionToInputCodesMap;
            HashMap<ActionIdentifier, Vector<TaggedActionCallback>> _actionCallbacks;
            HashMap<InputCode, TimeCondition> _inputCodeToTimedConditionsMap;

            Vector<ActionEvent> _actionEvents;
        };
        //--------------------------------------------------------------------------
    }
}