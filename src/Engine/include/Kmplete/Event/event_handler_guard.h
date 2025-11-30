#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/event_dispatcher.h"


namespace Kmplete
{
    template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
    class EventHandlerFunctionGuard
    {
        KMP_DISABLE_COPY_MOVE(EventHandlerFunctionGuard<EventClass>)

    public:
        EventHandlerFunctionGuard(EventDispatcher& eventDispatcher, const EventHandlerFunction<EventClass>& handlerFunction)
            : _eventDispatcher(eventDispatcher)
            , _handlerFunction(handlerFunction)
        {
            _eventDispatcher.AddHandler(_handlerFunction);
        }

        ~EventHandlerFunctionGuard()
        {
            _eventDispatcher.RemoveHandler(_handlerFunction);
        }

    private:
        EventDispatcher& _eventDispatcher;
        EventHandlerFunction<EventClass> _handlerFunction;
    };
    //--------------------------------------------------------------------------
}