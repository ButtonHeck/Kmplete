#include "Kmplete/Event/event_queue.h"


namespace Kmplete
{
    namespace Events
    {
        EventQueue& EventQueue::Get()
        {
            static EventQueue queue;
            return queue;
        }
        //--------------------------------------------------------------------------

        void EventQueue::QueueEvent(UPtr<Event>&& event)
        {
            _events.emplace_back(std::move(event));
        }
        //--------------------------------------------------------------------------

        Vector<UPtr<Event>>& EventQueue::GetEvents()
        {
            return _events;
        }
        //--------------------------------------------------------------------------
    }
}