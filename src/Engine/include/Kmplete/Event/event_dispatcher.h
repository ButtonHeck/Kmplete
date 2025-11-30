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
        KMP_DISABLE_COPY_MOVE(EventDispatcher)

    public:
        EventDispatcher() = default;

        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        void AddHandler(const EventHandler<EventClass>& newHandler)
        {
            if (!_handlersMap.contains(EventClass::TypeID))
            {
                _handlersMap.emplace(EventClass::TypeID, Vector<UPtr<EventHandlerWrapper>>());
            }

            const auto& handlers = _handlersMap[EventClass::TypeID];
            auto newHandlerWrapper = CreateUPtr<EventHandlerWrapperImpl<EventClass>>(newHandler);
            for (auto& handler : handlers)
            {
                if (handler->GetTypeName() == newHandlerWrapper->GetTypeName())
                {
                    KMP_LOG_ERROR("already contains exactly same handler for '{}'", EventClass::TypeName);
                    return;
                }
            }

            KMP_LOG_DEBUG("added handler for '{}' - {}", EventClass::TypeName, newHandlerWrapper->GetTypeName());
            _handlersMap[EventClass::TypeID].emplace_back(std::move(newHandlerWrapper));
        }

        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        void RemoveHandler(const EventHandler<EventClass>& handler)
        {
            if (!_handlersMap.contains(EventClass::TypeID))
            {
                KMP_LOG_WARN("cannot removed handler because its event TypeName '{}' has not been registered", EventClass::TypeName);
                return;
            }

            auto& handlers = _handlersMap[EventClass::TypeID];
            const auto handlerTypeName = handler.target_type().name();
            for (auto handlerIter = handlers.begin(); handlerIter != handlers.end(); handlerIter++)
            {
                if (handlerIter->get()->GetTypeName() == handlerTypeName)
                {
                    KMP_LOG_DEBUG("removed handler for '{}' - {}", EventClass::TypeName, handlerTypeName);
                    handlers.erase(handlerIter);
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
        HashMap<EventTypeID, Vector<UPtr<EventHandlerWrapper>>> _handlersMap;
    };
    //--------------------------------------------------------------------------
}