#include "Kmplete/Input/input_manager.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Event/key_events.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Input
    {
        InputManager::InputManager() noexcept
            : _mousePosition(0, 0)
            , _mouseButtonsStates({false})
            , _keyButtonsStates({false})
            , _modifiersMask(Modifier::None)
            , _inputCodeToActionsMap({})
            , _actionToInputCodesMap({})
            , _actionCallbacks({})
        {}
        //--------------------------------------------------------------------------

        void InputManager::ProcessInputEvents(Events::Event& event)
        {
            const auto eventTypeID = event.GetTypeID();

            if (eventTypeID == Events::MouseMoveEventTypeID)
            {
                auto& mouseMoveEvent = static_cast<Events::MouseMoveEvent&>(event);
                _mousePosition = Math::Point2I(mouseMoveEvent.GetX(), mouseMoveEvent.GetY());
            }

            else if (eventTypeID == Events::MouseButtonPressEventTypeID)
            {
                const auto& mouseButtonPressEvent = static_cast<Events::MouseButtonPressEvent&>(event);
                const auto mouseButton = mouseButtonPressEvent.GetMouseButton();

                _mouseButtonsStates[mouseButton] = 1.0f;

                const auto newEvents = _CreateActionEvents(mouseButton, 1.0f);
                if (!newEvents.empty())
                {
                    std::move(newEvents.begin(), newEvents.end(), std::inserter(_actionEvents, _actionEvents.end()));
                }
            }
            else if (eventTypeID == Events::MouseButtonReleaseEventTypeID)
            {
                const auto& mouseButtonReleaseEvent = static_cast<Events::MouseButtonReleaseEvent&>(event);
                const auto mouseButton = mouseButtonReleaseEvent.GetMouseButton();

                _mouseButtonsStates[mouseButton] = 0.0f;

                const auto newEvents = _CreateActionEvents(mouseButton, 0.0f);
                if (!newEvents.empty())
                {
                    std::move(newEvents.begin(), newEvents.end(), std::inserter(_actionEvents, _actionEvents.end()));
                }
            }

            else if (eventTypeID == Events::KeyPressEventTypeID)
            {
                const auto& keyPressEvent = static_cast<Events::KeyPressEvent&>(event);
                const auto keyCode = keyPressEvent.GetKeyCode();
                const auto modifiers = keyPressEvent.GetMods();

                _keyButtonsStates[keyCode] = 1.0f;
                _modifiersMask = modifiers;

                if (!keyPressEvent.IsRepeat())
                {
                    const auto newEvents = _CreateActionEvents(keyCode, 1.0f);
                    if (!newEvents.empty())
                    {
                        std::move(newEvents.begin(), newEvents.end(), std::inserter(_actionEvents, _actionEvents.end()));
                    }
                }
            }
            else if (eventTypeID == Events::KeyReleaseEventTypeID)
            {
                const auto& keyReleaseEvent = static_cast<Events::KeyReleaseEvent&>(event);
                const auto keyCode = keyReleaseEvent.GetKeyCode();
                const auto modifiers = keyReleaseEvent.GetMods();

                _keyButtonsStates[keyCode] = 0.0f;
                _modifiersMask = modifiers;

                const auto newEvents = _CreateActionEvents(keyCode, 0.0f);
                if (!newEvents.empty())
                {
                    std::move(newEvents.begin(), newEvents.end(), std::inserter(_actionEvents, _actionEvents.end()));
                }
            }
        }
        //--------------------------------------------------------------------------

        void InputManager::PropagateActionEvents()
        {
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

        bool InputManager::IsMouseButtonPressed(MouseCode mouseCode) const
        {
            return _mouseButtonsStates[mouseCode];
        }
        //--------------------------------------------------------------------------

        KeyModifier InputManager::GetKeyModifiersMask() const noexcept
        {
            return _modifiersMask;
        }
        //--------------------------------------------------------------------------

        bool InputManager::IsKeyButtonPressed(KeyCode keyCode) const
        {
            return _keyButtonsStates[keyCode];
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
            if (_inputCodeToActionsMap.contains(code) && _ContainsActionIdentifier(_inputCodeToActionsMap[code], actionId))
            {
                KMP_LOG_WARN("input code '{}' already mapped to action ID '{}'", code, actionId);
                return false;
            }

            _inputCodeToActionsMap[code].emplace_back(actionId);
            _actionToInputCodesMap[actionId].emplace_back(code);

            return true;
        }
        //--------------------------------------------------------------------------

        bool InputManager::UnmapInputFromAction(InputCode code, ActionIdentifier actionId)
        {
            const auto actionsErased = std::erase_if(_inputCodeToActionsMap[code], [actionId](const ActionIdentifier& actionInMap) {
                return actionInMap == actionId;
            });

            const auto inputsErased = std::erase_if(_actionToInputCodesMap[actionId], [code](const InputCode& codeInMap) {
                return codeInMap == code;
            });

            if (actionsErased == 0 || inputsErased == 0)
            {
                KMP_LOG_WARN("failed to unmap input code '{}' from action '{}'", code, actionId);
                return false;
            }

            return true;
        }
        //--------------------------------------------------------------------------

        bool InputManager::RemapInputToAction(InputCode code, ActionIdentifier actionId)
        {
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

            UnmapInputFromAction(_actionToInputCodesMap[actionId].front(), actionId);
            return MapInputToAction(code, actionId);
        }
        //--------------------------------------------------------------------------

        InputControlValue InputManager::GetActionValue(ActionIdentifier actionId)
        {
            if (!_actionToInputCodesMap.contains(actionId))
            {
                return 0.0f;
            }

            const auto& codes = _actionToInputCodesMap[actionId];
            InputControlValue resultValue = 0.0f;
            for (const auto& code : codes)
            {
                if (code < Mouse::NumButtons)
                {
                    const auto currentMouseValue = _mouseButtonsStates[code];
                    if (std::abs(currentMouseValue) > std::abs(resultValue))
                    {
                        resultValue = currentMouseValue;
                    }
                }
                else if (code >= Key::First && code < Key::NumKeys)
                {
                    const auto currentKeyboardValue = _keyButtonsStates[code];
                    if (std::abs(currentKeyboardValue) > std::abs(resultValue))
                    {
                        resultValue = currentKeyboardValue;
                    }
                }
            }

            return resultValue;
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapInputToCallback(InputCode code, ActionIdentifier actionId, const ActionCallback& callback)
        {
            const auto inputMapped = MapInputToAction(code, actionId);
            if (!inputMapped)
            {
                return false;
            }

            return MapActionToCallback(actionId, callback);
        }
        //--------------------------------------------------------------------------

        bool InputManager::MapInputToCallback(InputCode code, ActionIdentifier actionId, const TaggedActionCallback& taggedCallback)
        {
            const auto inputMapped = MapInputToAction(code, actionId);
            if (!inputMapped)
            {
                return false;
            }

            return MapActionToCallback(actionId, taggedCallback);
        }
        //--------------------------------------------------------------------------

        Vector<ActionEvent> InputManager::_CreateActionEvents(InputCode code, InputControlValue value) const
        {
            Vector<ActionEvent> actionEvents;
            if (!_inputCodeToActionsMap.contains(code))
            {
                return actionEvents;
            }

            const auto& actionIds = _inputCodeToActionsMap.at(code);
            for (const auto& actionId : actionIds)
            {
                actionEvents.emplace_back(ActionEvent{
                    .id = actionId,
                    .value = value
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