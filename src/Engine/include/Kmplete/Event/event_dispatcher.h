#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/event_handler.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class EventDispatcher
    {
        KMP_LOG_CLASSNAME(EventDispatcher)

    public:
        EventDispatcher() = default;

        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        void AddHandler(const EventHandlerFunction<EventClass>& handler)
        {
            if (!_handlersMap.contains(EventClass::staticTypeID))
            {
                _handlersMap.emplace(EventClass::staticTypeID, Vector<UPtr<EventHandler>>());
            }

            const auto& handlers = _handlersMap[EventClass::staticTypeID];
            auto newHandler = CreateUPtr<EventHandlerImpl<EventClass>>(handler);
            for (auto& registeredHandler : handlers)
            {
                if (registeredHandler->GetTypeName() == newHandler->GetTypeName())
                {
                    KMP_LOG_ERROR("already contains exactly same handler for '{}'", EventClass::staticTypeID);
                    return;
                }
            }

            _handlersMap[EventClass::staticTypeID].emplace_back(std::move(newHandler));
        }

        bool Dispatch(Event& event)
        {
            const auto eventTypeID = event.GetTypeID();
            if (!_handlersMap.contains(eventTypeID) || event.handled)
            {
                return false;
            }

            auto& handlers = _handlersMap[eventTypeID];
            for (const auto& handler : handlers)
            {
                event.handled |= handler->ProcessEvent(event);
            }

            return true;
        }

    private:
        HashMap<EventTypeID, Vector<UPtr<EventHandler>>> _handlersMap;
    };
    //--------------------------------------------------------------------------
}