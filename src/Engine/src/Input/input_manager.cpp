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
            , _keyModes(Mode::None)
        {}
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

        KeyMode InputManager::GetKeyModes() const noexcept
        {
            return _keyModes;
        }
        //--------------------------------------------------------------------------

        bool InputManager::IsKeyButtonPressed(KeyCode keyCode) const
        {
            return _keyButtonsStates[keyCode];
        }
        //--------------------------------------------------------------------------

        void InputManager::OnEvent(Events::Event& event)
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
                _mouseButtonsStates[mouseButtonPressEvent.GetMouseButton()] = true;
            }
            else if (eventTypeID == Events::MouseButtonReleaseEventTypeID)
            {
                const auto& mouseButtonReleaseEvent = static_cast<Events::MouseButtonReleaseEvent&>(event);
                _mouseButtonsStates[mouseButtonReleaseEvent.GetMouseButton()] = false;
            }

            else if (eventTypeID == Events::KeyPressEventTypeID)
            {
                const auto& keyPressEvent = static_cast<Events::KeyPressEvent&>(event);
                _keyButtonsStates[keyPressEvent.GetKeyCode()] = true;
                _keyModes = keyPressEvent.GetMods();
            }
            else if (eventTypeID == Events::KeyReleaseEventTypeID)
            {
                const auto& keyReleaseEvent = static_cast<Events::KeyReleaseEvent&>(event);
                _keyButtonsStates[keyReleaseEvent.GetKeyCode()] = false;
                _keyModes = keyReleaseEvent.GetMods();
            }
        }
        //--------------------------------------------------------------------------
    }
}