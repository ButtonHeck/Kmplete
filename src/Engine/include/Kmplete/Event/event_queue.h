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


        inline void QueueEvent(UPtr<Event>&& event)
        {
            EventQueue::Get().QueueEvent(std::move(event));
        }
        //--------------------------------------------------------------------------
    }
}