#include "Kmplete/Input/input_manager.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Event/key_events.h"


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
            const auto& callbackWrappers = _actionCallbacks[actionEvent.id];
            for (size_t i = 0; callbackWrappers.size(); i++)
            {
                auto& wrapper = callbackWrappers[i];
                const auto eventProcessed = wrapper.callback(actionEvent.value);
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

        void InputManager::AddActionCallback(ActionIdentifier actionId, const ActionCallbackWrapper& callbackWrapper)
        {
            _actionCallbacks[actionId].emplace_back(callbackWrapper);
        }
        //--------------------------------------------------------------------------

        void InputManager::RemoveActionCallback(ActionIdentifier actionId, const ActionCallbackIdentifier& callbackId)
        {
            std::erase_if(_actionCallbacks[actionId], [callbackId](const ActionCallbackWrapper& callbackWrapper) {
                return callbackWrapper.id == callbackId;
            });
        }
        //--------------------------------------------------------------------------

        void InputManager::MapInputToAction(InputCode code, ActionIdentifier actionId)
        {
            _inputCodeToActionsMap[code].emplace_back(actionId);
            _actionToInputCodesMap[actionId].emplace_back(code);
        }
        //--------------------------------------------------------------------------

        void InputManager::UnmapInputFromAction(InputCode code, ActionIdentifier actionId)
        {
            std::erase_if(_inputCodeToActionsMap[code], [actionId](const ActionIdentifier& actionInMap) {
                return actionInMap == actionId;
            });

            std::erase_if(_actionToInputCodesMap[actionId], [code](const InputCode& codeInMap) {
                return codeInMap == code;
            });
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
    }
}