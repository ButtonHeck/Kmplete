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
            if (!_handlersMap.contains(EventClass::TypeID))
            {
                _handlersMap.emplace(EventClass::TypeID, Vector<UPtr<EventHandler>>());
            }

            const auto& registeredHandlers = _handlersMap[EventClass::TypeID];
            auto newHandler = CreateUPtr<EventHandlerImpl<EventClass>>(handler);
            for (auto& registeredHandler : registeredHandlers)
            {
                if (registeredHandler->GetTypeName() == newHandler->GetTypeName())
                {
                    KMP_LOG_ERROR("already contains exactly same handler for '{}'", EventClass::TypeName);
                    return;
                }
            }

            KMP_LOG_DEBUG("added handler for '{}' - {}", EventClass::TypeName, newHandler->GetTypeName());
            _handlersMap[EventClass::TypeID].emplace_back(std::move(newHandler));
        }

        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        void RemoveHandler(const EventHandlerFunction<EventClass>& handler)
        {
            if (!_handlersMap.contains(EventClass::TypeID))
            {
                KMP_LOG_WARN("cannot removed handler because its event TypeName '{}' has not been registered", EventClass::TypeName);
                return;
            }

            auto& registeredHandlers = _handlersMap[EventClass::TypeID];
            const auto handlerTypeName = handler.target_type().name();
            for (auto it = registeredHandlers.begin(); it != registeredHandlers.end(); it++)
            {
                if (it->get()->GetTypeName() == handlerTypeName)
                {
                    KMP_LOG_DEBUG("removed handler for '{}' - {}", EventClass::TypeName, handlerTypeName);
                    registeredHandlers.erase(it);
                    return;
                }
            }
        }

        bool Dispatch(Event& event)
        {
            const auto eventTypeID = event.GetTypeID();
            if (!_handlersMap.contains(eventTypeID) || event.handled)
            {
                return false;
            }

            auto& handlers = _handlersMap[eventTypeID];
            for (size_t handlerIndex = 0; handlerIndex < handlers.size(); handlerIndex++)
            {
                const auto& handler = handlers[handlerIndex];
                event.handled |= handler->ProcessEvent(event);
            }

            return true;
        }

    private:
        HashMap<EventTypeID, Vector<UPtr<EventHandler>>> _handlersMap;
    };
    //--------------------------------------------------------------------------
}