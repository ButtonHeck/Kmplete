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
                auto& mouseMoveEvent = static_cast<Events::MouseMoveEvent&>(event);
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

            else if (eventTypeID == Events::MouseButtonPressEventTypeID)
            {
                const auto& mouseButtonPressEvent = static_cast<Events::MouseButtonPressEvent&>(event);
                const auto mouseButton = mouseButtonPressEvent.GetMouseButton();

                _controlStates[mouseButton] = ButtonPressed;

                const auto newEvents = _CreateActionEvents(mouseButton, ButtonPressed);
                Utils::MergeVectors<ActionEvent>(newEvents, _actionEvents);
            }
            else if (eventTypeID == Events::MouseButtonReleaseEventTypeID)
            {
                const auto& mouseButtonReleaseEvent = static_cast<Events::MouseButtonReleaseEvent&>(event);
                const auto mouseButton = mouseButtonReleaseEvent.GetMouseButton();

                _controlStates[mouseButton] = ButtonReleased;

                const auto newEvents = _CreateActionEvents(mouseButton, ButtonReleased);
                Utils::MergeVectors<ActionEvent>(newEvents, _actionEvents);
            }

            else if (eventTypeID == Events::KeyPressEventTypeID)
            {
                const auto& keyPressEvent = static_cast<Events::KeyPressEvent&>(event);
                const auto keyCode = keyPressEvent.GetKeyCode();
                const auto modifiers = keyPressEvent.GetMods();

                _controlStates[keyCode] = ButtonPressed;
                _modifiersMask = modifiers;

                if (!keyPressEvent.IsRepeat())
                {
                    const auto newEvents = _CreateActionEvents(keyCode, ButtonPressed);
                    Utils::MergeVectors<ActionEvent>(newEvents, _actionEvents);
                }
            }
            else if (eventTypeID == Events::KeyReleaseEventTypeID)
            {
                const auto& keyReleaseEvent = static_cast<Events::KeyReleaseEvent&>(event);
                const auto keyCode = keyReleaseEvent.GetKeyCode();
                const auto modifiers = keyReleaseEvent.GetMods();

                _controlStates[keyCode] = ButtonReleased;
                _modifiersMask = modifiers;

                const auto newEvents = _CreateActionEvents(keyCode, ButtonReleased);
                Utils::MergeVectors<ActionEvent>(newEvents, _actionEvents);
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
                    if (codeWithCondition.condition.value != EmptyValue && value != codeWithCondition.condition.value)
                    {
                        return;
                    }

                    if (codeWithCondition.condition.modifierMask == Input::Modifier::None || (_modifiersMask & codeWithCondition.condition.modifierMask) == codeWithCondition.condition.modifierMask)
                    {
                        actionEvents.emplace_back(ActionEvent{
                            .id = actionId,
                            .value = value
                        });
                    }
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