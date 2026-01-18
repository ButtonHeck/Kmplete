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
            std::lock_guard<std::mutex> lock(_mutex);
            _events.emplace_back(std::move(event));
        }
        //--------------------------------------------------------------------------

        Vector<UPtr<Event>>& EventQueue::GetEvents()
        {
            //! No need to use mutex as it is called only from FrameListenerManager
            return _events;
        }
        //--------------------------------------------------------------------------
    }
}