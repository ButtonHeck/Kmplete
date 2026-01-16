#include "Kmplete/Input/input_manager.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Event/key_events.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Input
    {
        InputManager::InputManager() noexcept
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _mousePosition(0, 0)
            , _controlStates({false})
            , _modifiersMask(Modifier::None)
            , _inputCodeToActionsMap({})
            , _actionToInputCodesMap({})
            , _actionCallbacks({})
        {
            _controlStates[Code::Mouse_Move] = Math::Point2I();
            _controlStates[Code::Mouse_Position] = Math::Point2I();
        }
        //--------------------------------------------------------------------------

        void InputManager::ProcessInputEvents(Events::Event& event)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto eventTypeID = event.GetTypeID();

            if (eventTypeID == Events::MouseMoveEventTypeID)
            {
                _ProcessMouseMoveEvent(static_cast<Events::MouseMoveEvent&>(event));
            }

            else if (eventTypeID == Events::MouseButtonPressEventTypeID)
            {
                _ProcessMouseButtonPressEvent(static_cast<Events::MouseButtonPressEvent&>(event));
            }
            else if (eventTypeID == Events::MouseButtonReleaseEventTypeID)
            {
                _ProcessMouseButtonReleaseEvent(static_cast<Events::MouseButtonReleaseEvent&>(event));
            }

            else if (eventTypeID == Events::KeyPressEventTypeID)
            {
                _ProcessKeyPressEvent(static_cast<Events::KeyPressEvent&>(event));
            }
            else if (eventTypeID == Events::KeyReleaseEventTypeID)
            {
                _ProcessKeyReleaseEvent(static_cast<Events::KeyReleaseEvent&>(event));
            }
        }
        //--------------------------------------------------------------------------

        void InputManager::PropagateActionEvents()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            for (const auto& actionEvent : _actionEvents)
            {
                if (_actionCallbacks[actionEvent.id].empty())
                {
                    continue;
                }

                _PropagateSingleActionEvent(actionEvent);
            }

            _actionEvents.clear();
        }
        //--------------------------------------------------------------------------

        void InputManager::_PropagateSingleActionEvent(const ActionEvent& actionEvent)
        {
            const auto& taggedCallbacks = _actionCallbacks[actionEvent.id];
            for (size_t i = 0; taggedCallbacks.size(); i++)
            {
                auto& taggedCallback = taggedCallbacks[i];
                const auto eventProcessed = taggedCallback.callback(actionEvent.value);
                if (eventProcessed)
                {
                    return;
                }
            }
        }
        //--------------------------------------------------------------------------

        const Math::Point2I& InputManager::GetMousePosition() const noexcept
        {
            return _mousePosition;
        }
        //--------------------------------------------------------------------------

        bool InputManager::IsMouseButtonPressed(InputCode mouseCode) const
        {
            return _controlStates[mouseCode] == ButtonPressedValue;
        }
        //--------------------------------------------------------------------------

        KeyModifier InputManager::GetKeyModifiersMask() const noexcept
        {
            return _modifiersMask;
        }
        //--------------------------------------------------------------------------

        bool InputManager::IsKeyButtonPressed(InputCode keyCode) const
        {
            return _controlStates[keyCode] == ButtonPressedValue;
        }
        //--------------------------------------------------------------------------

        void InputManager::ResetMouseMove() noexcept
        {
            _controlStates[Code::Mouse_Move] = Math::Point2I();
        }
        //--------------------------------------------------------------------------

        void InputManager::UpdateTimerActions(float frameTimestep)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            for (auto& [code, timerCondition] : _inputCodeToTimedConditionsMap)
            {
                if (timerCondition.active && _controlStates[code] == ButtonPressedValue)
                {
                    timerCondition.currentMs += frameTimestep;

                    if (timerCondition.currentMs >= timerCondition.triggerMs)
                    {
                        const auto newEvents = _CreateActionEvents(code, ButtonPressed);
                        Utils::MergeVectors<ActionEvent>(newEvents, _actionEvents);

                        timerCondition.Deactivate();
                    }
                }
            }
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapActionToCallback(ActionIdentifier actionId, const ActionCallback& callback)
        {
            TaggedActionCallback taggedCallback{
                .tag = DefaultActionCallbackTag,
                .callback = callback
            };

            return MapActionToCallback(actionId, taggedCallback);
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapActionToCallback(ActionIdentifier actionId, const TaggedActionCallback& taggedCallback)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            if (_actionCallbacks.contains(actionId) && _ContainsTaggedCallback(_actionCallbacks[actionId], taggedCallback))
            {
                KMP_LOG_WARN("already contains callback with same action ID '{}' and callback tag '{}'", actionId, taggedCallback.tag);
                return false;
            }

            _actionCallbacks[actionId].emplace_back(taggedCallback);
            return true;
        }
        //--------------------------------------------------------------------------

        bool InputManager::UnmapActionFromCallback(ActionIdentifier actionId, const ActionCallbackTag& callbackTag)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            const auto erasedCount = std::erase_if(_actionCallbacks[actionId], [callbackTag](const TaggedActionCallback& taggedCallback) {
                return taggedCallback.tag == callbackTag;
            });

            if (erasedCount == 0)
            {
                KMP_LOG_WARN("failed to unmap callback with tag '{}' from action '{}'", callbackTag, actionId);
                return false;
            }

            return true;
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapInputToAction(InputCode code, ActionIdentifier actionId)
        {
            return MapInputToAction(InputCodeWithCondition{.code = code, .condition = NoCondition}, actionId);
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapInputToAction(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            const auto inputCode = codeWithCondition.code;
            if (_inputCodeToActionsMap.contains(inputCode) && _ContainsActionIdentifier(_inputCodeToActionsMap[inputCode], actionId))
            {
                KMP_LOG_WARN("input code '{}' already mapped to action ID '{}'", inputCode, actionId);
                return false;
            }

            _inputCodeToActionsMap[inputCode].emplace_back(actionId);
            _actionToInputCodesMap[actionId].emplace_back(codeWithCondition);

            if (codeWithCondition.condition.timerTriggerMs > 0.0f)
            {
                _inputCodeToTimedConditionsMap[inputCode] = TimeCondition{ .active = false, .currentMs = 0.0f, .triggerMs = codeWithCondition.condition.timerTriggerMs };
            }

            return true;
        }
        //--------------------------------------------------------------------------

        bool InputManager::UnmapInputFromAction(InputCode code, ActionIdentifier actionId)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            const auto actionsErased = std::erase_if(_inputCodeToActionsMap[code], [actionId](const ActionIdentifier& actionInMap) {
                return actionInMap == actionId;
            });

            const auto inputsErased = std::erase_if(_actionToInputCodesMap[actionId], [code](const InputCodeWithCondition& codeWithCondition) {
                return codeWithCondition.code == code;
            });

            if (actionsErased == 0 || inputsErased == 0)
            {
                KMP_LOG_WARN("failed to unmap input code '{}' from action '{}'", code, actionId);
                return false;
            }

            _inputCodeToTimedConditionsMap.erase(code);

            return true;
        }
        //--------------------------------------------------------------------------

        bool InputManager::RemapInputToAction(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            if (!_actionToInputCodesMap.contains(actionId))
            {
                KMP_LOG_WARN("action '{}' is not registered", actionId);
                return false;
            }

            if (_actionToInputCodesMap[actionId].size() != 1)
            {
                KMP_LOG_WARN("cannot remap to '{}' due to multiple input codes binding, use Unmap(old code)+Map(new code) directly", actionId);
                return false;
            }

            UnmapInputFromAction(_actionToInputCodesMap[actionId].front().code, actionId);
            return MapInputToAction(codeWithCondition, actionId);
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapInputToCallback(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId, const ActionCallback& callback)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            const auto inputMapped = MapInputToAction(codeWithCondition, actionId);
            if (!inputMapped)
            {
                return false;
            }

            return MapActionToCallback(actionId, callback);
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapInputToCallback(InputCodeWithCondition codeWithCondition, ActionIdentifier actionId, const TaggedActionCallback& taggedCallback)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            const auto inputMapped = MapInputToAction(codeWithCondition, actionId);
            if (!inputMapped)
            {
                return false;
            }

            return MapActionToCallback(actionId, taggedCallback);
        }
        //--------------------------------------------------------------------------

        void InputManager::_ProcessMouseMoveEvent(const Events::MouseMoveEvent& mouseMoveEvent)
        {
            const auto newMousePosition = Math::Point2I(mouseMoveEvent.GetX(), mouseMoveEvent.GetY());
            const auto mouseMove = newMousePosition - _mousePosition;

            const auto moveEvents = _CreateActionEvents(Input::Code::Mouse_Move, mouseMove);
            Utils::MergeVectors<ActionEvent>(moveEvents, _actionEvents);

            const auto positionEvents = _CreateActionEvents(Input::Code::Mouse_Position, newMousePosition);
            Utils::MergeVectors<ActionEvent>(positionEvents, _actionEvents);

            _mousePosition = newMousePosition;
            _controlStates[Code::Mouse_Position] = _mousePosition;
            _controlStates[Code::Mouse_Move] = mouseMove;
        }
        //--------------------------------------------------------------------------

        void InputManager::_ProcessMouseButtonPressEvent(const Events::MouseButtonPressEvent& mouseButtonPressEvent)
        {
            const auto mouseButton = mouseButtonPressEvent.GetMouseButton();

            _controlStates[mouseButton] = ButtonPressed;

            _UpdateActionEvents(mouseButton, ButtonPressed, true);
        }
        //--------------------------------------------------------------------------

        void InputManager::_ProcessMouseButtonReleaseEvent(const Events::MouseButtonReleaseEvent& mouseButtonReleaseEvent)
        {
            const auto mouseButton = mouseButtonReleaseEvent.GetMouseButton();

            _controlStates[mouseButton] = ButtonReleased;

            _UpdateActionEvents(mouseButton, ButtonReleased, false);
        }
        //--------------------------------------------------------------------------

        void InputManager::_ProcessKeyPressEvent(const Events::KeyPressEvent& keyPressEvent)
        {
            const auto keyCode = keyPressEvent.GetKeyCode();
            const auto modifiers = keyPressEvent.GetMods();

            _controlStates[keyCode] = ButtonPressed;
            _modifiersMask = modifiers;

            if (!keyPressEvent.IsRepeat())
            {
                _UpdateActionEvents(keyCode, ButtonPressed, true);
            }
        }
        //--------------------------------------------------------------------------

        void InputManager::_ProcessKeyReleaseEvent(const Events::KeyReleaseEvent& keyReleaseEvent)
        {
            const auto keyCode = keyReleaseEvent.GetKeyCode();
            const auto modifiers = keyReleaseEvent.GetMods();

            _controlStates[keyCode] = ButtonReleased;
            _modifiersMask = modifiers;

            _UpdateActionEvents(keyCode, ButtonReleased, false);
        }
        //--------------------------------------------------------------------------

        void InputManager::_UpdateActionEvents(InputCode code, InputControlValue value, bool isActivation)
        {
            const auto newEvents = _CreateActionEvents(code, value);
            Utils::MergeVectors<ActionEvent>(newEvents, _actionEvents);

            if (_inputCodeToTimedConditionsMap.contains(code))
            {
                isActivation
                    ? _inputCodeToTimedConditionsMap[code].Activate()
                    : _inputCodeToTimedConditionsMap[code].Deactivate();
            }
        }
        //--------------------------------------------------------------------------

        Vector<ActionEvent> InputManager::_CreateActionEvents(InputCode code, InputControlValue value) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            Vector<ActionEvent> actionEvents;
            if (!_inputCodeToActionsMap.contains(code))
            {
                return actionEvents;
            }

            const auto& actionIds = _inputCodeToActionsMap.at(code);
            for (const auto& actionId : actionIds)
            {
                const auto& conditions = _actionToInputCodesMap.at(actionId);
                std::for_each(conditions.cbegin(), conditions.cend(), [&](const InputCodeWithCondition& codeWithCondition) {
                    if ((codeWithCondition.condition.value != EmptyValue && value != codeWithCondition.condition.value) ||
                        (codeWithCondition.condition.modifierMask != Input::Modifier::None && (_modifiersMask & codeWithCondition.condition.modifierMask) != codeWithCondition.condition.modifierMask) ||
                        (codeWithCondition.condition.timerTriggerMs > 0.0f && _inputCodeToTimedConditionsMap.contains(code) && _inputCodeToTimedConditionsMap.at(code).currentMs < codeWithCondition.condition.timerTriggerMs))
                    {
                        return;
                    }

                    actionEvents.emplace_back(ActionEvent{
                        .id = actionId,
                        .value = value
                    });
                });
            }

            return actionEvents;
        }
        //--------------------------------------------------------------------------

        bool InputManager::_ContainsTaggedCallback(const Vector<TaggedActionCallback>& callbacks, const TaggedActionCallback& taggedCallback) const
        {
            return std::find_if(callbacks.cbegin(), callbacks.cend(), [taggedCallback](const TaggedActionCallback& callbackInVector) {
                return callbackInVector.tag == taggedCallback.tag;
            }) != callbacks.cend();
        }
        //--------------------------------------------------------------------------

        bool InputManager::_ContainsActionIdentifier(const Vector<ActionIdentifier>& actionsIdentifiers, ActionIdentifier actionId) const
        {
            return std::find(actionsIdentifiers.cbegin(), actionsIdentifiers.cend(), actionId) != actionsIdentifiers.cend();
        }
        //--------------------------------------------------------------------------
    }
}