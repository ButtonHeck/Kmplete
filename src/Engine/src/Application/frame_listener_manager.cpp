#include "Kmplete/Application/frame_listener_manager.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    FrameListenerManager::~FrameListenerManager()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _listeners.clear();
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::AddFrameListener(NonNull<FrameListener*> frameListener)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _listeners.emplace_back(FrameListenerWrapper(frameListener, true));
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto& listener : _listeners)
        {
            if (listener.isActive)
            {
                listener.frameListener->Update(frameTimestep, mainWindowIsIconified);
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::RenderFrameListeners()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto& listener : _listeners)
        {
            if (listener.isActive)
            {
                listener.frameListener->Render();
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::ProcessEventsFrameListeners(Event& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto iter = _listeners.rbegin(); iter != _listeners.rend(); ++iter)
        {
            auto listener = *iter;
            if (listener.isActive)
            {
                if (event.handled)
                {
                    break;
                }

                listener.frameListener->OnEvent(event);
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::ProcessFrameListenersCommands()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _commandBuffer.clear();
    }
    //--------------------------------------------------------------------------
}