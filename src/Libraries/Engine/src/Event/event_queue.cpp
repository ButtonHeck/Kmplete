#include "Kmplete/Event/event_queue.h"
#include "Kmplete/Profile/profiler.h"


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

        void EventQueue::QueueEvent(UPtr<Event>&& event) KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _events.emplace_back(std::move(event));
        }}
        //--------------------------------------------------------------------------

        Vector<UPtr<Event>> EventQueue::GetEvents() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            Vector<UPtr<Event>> events;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                events = std::move(_events);
                _events.clear();
            }
            
            return events;
        }}
        //--------------------------------------------------------------------------
    }
}