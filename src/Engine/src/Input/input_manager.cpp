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
        }
        //--------------------------------------------------------------------------
    }
}