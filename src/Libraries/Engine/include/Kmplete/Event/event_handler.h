#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    namespace Events
    {
        //! Alias for event handler function signature
        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        using EventHandler = Function<bool(EventClass&)>;


        //! Typeless wrapper of event handler for putting in some storage without
        //! defining template parameters
        //! @see Events::EventDispatcher
        class EventHandlerWrapper
        {
        public:
            virtual ~EventHandlerWrapper() = default;

            virtual bool ProcessEvent(Event& event) const = 0;
            KMP_NODISCARD virtual const String& GetTypeName() const = 0;
        };
        //--------------------------------------------------------------------------


        //! Typed event handler wrapper that processes specific type of event
        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        class EventHandlerWrapperImpl : public EventHandlerWrapper
        {
            KMP_DISABLE_COPY_MOVE(EventHandlerWrapperImpl)

        public:
            explicit EventHandlerWrapperImpl(const EventHandler<EventClass>& handler) noexcept
                : _handler(handler)
                , _typeName(_handler.target_type().name())
            {}

            bool ProcessEvent(Event& event) const override
            {
                if (event.GetTypeID() == EventClass::TypeID)
                {
                    return _handler(static_cast<EventClass&>(event));
                }

                return false;
            }

            KMP_NODISCARD const String& GetTypeName() const noexcept override
            {
                return _typeName;
            }

        private:
            EventHandler<EventClass> _handler;
            const String _typeName;
        };
        //--------------------------------------------------------------------------
    }
}