#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Event/event.h"

#include <functional>


namespace Kmplete
{
    template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
    using EventHandlerFunction = std::function<bool(EventClass&)>;


    class EventHandler
    {
    public:
        virtual ~EventHandler() = default;

        virtual bool ProcessEvent(Event& event) const = 0;
    };
    //--------------------------------------------------------------------------


    template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
    class EventHandlerImpl : public EventHandler
    {
    public:
        explicit EventHandlerImpl(const EventHandlerFunction<EventClass>& handler) noexcept
            : _handlerFunction(handler)
        {}

        bool ProcessEvent(Event& event) const override
        {
            if (event.GetTypeID() == EventClass::staticTypeID)
            {
                return _handlerFunction(static_cast<EventClass&>(event));
            }

            return false;
        }

    private:
        EventHandlerFunction<EventClass> _handlerFunction;
    };
    //--------------------------------------------------------------------------
}