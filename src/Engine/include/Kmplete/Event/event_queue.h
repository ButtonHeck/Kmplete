#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class FrameListenerManager;


    namespace Events
    {
        //! "Global" event queue class that may be asked to queue any event object,
        //! additionally FrameListenerManager class is allowed to get stored events for
        //! further processing (the queue itself doesn't do anything other than collecting events).
        //! This is a somewhat compromise between decoupling parts of the engine on the one hand
        //! and hiding possibility of accidental stealing events from any other part of code on the other hand.
        //! @see FrameListenerManager
        //! @see Events::Event
        class EventQueue
        {
            KMP_DISABLE_COPY_MOVE(EventQueue)

        public:
            KMP_NODISCARD KMP_API static EventQueue& Get();

        public:
            KMP_API void QueueEvent(UPtr<Event>&& event);

        private:
            EventQueue() = default;
            ~EventQueue() = default;

            friend class ::Kmplete::FrameListenerManager;

            KMP_NODISCARD Vector<UPtr<Event>>& GetEvents();

        private:
            Vector<UPtr<Event>> _events;
        };
        //--------------------------------------------------------------------------


        //! Shortcut to queue an event
        inline void QueueEvent(UPtr<Event>&& event)
        {
            EventQueue::Get().QueueEvent(std::move(event));
        }
        //--------------------------------------------------------------------------
    }
}