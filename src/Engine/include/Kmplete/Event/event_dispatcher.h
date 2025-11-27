#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/event_handler.h"


namespace Kmplete
{
    class EventDispatcher
    {
    public:
        EventDispatcher() = default;

        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        void AddHandler(const EventHandlerFunction<EventClass>& handler)
        {
            // TODO: check double addition
            _handlers.emplace(EventClass::staticTypeID, CreateUPtr<EventHandlerImpl<EventClass>>(handler));
        }

        bool Dispatch(Event& event)
        {
            if (!_handlers.contains(event.GetTypeID()) || event.handled)
            {
                return false;
            }

            event.handled |= _handlers[event.GetTypeID()]->ProcessEvent(event);
            return true;
        }

    private:
        HashMap<EventTypeID, UPtr<EventHandler>> _handlers;
    };
    //--------------------------------------------------------------------------
}