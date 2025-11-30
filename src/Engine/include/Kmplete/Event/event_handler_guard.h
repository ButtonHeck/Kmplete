#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/event_dispatcher.h"


namespace Kmplete
{
    template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
    class EventHandlerGuard
    {
        KMP_DISABLE_COPY_MOVE(EventHandlerGuard<EventClass>)

    public:
        EventHandlerGuard(EventDispatcher& eventDispatcher, const EventHandler<EventClass>& handler, bool attachOnCreate = true) noexcept
            : _eventDispatcher(eventDispatcher)
            , _handler(handler)
        {
            if (attachOnCreate)
            {
                Attach();
            }
        }

        ~EventHandlerGuard()
        {
            Detach();
        }

        void Attach()
        {
            _eventDispatcher.AddHandler(_handler);
        }

        void Detach()
        {
            _eventDispatcher.RemoveHandler(_handler);
        }

    private:
        EventDispatcher& _eventDispatcher;
        EventHandler<EventClass> _handler;
    };
    //--------------------------------------------------------------------------
}