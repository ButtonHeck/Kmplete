#include "Kmplete/Application/frame_listener_manager.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    FrameListenerManager::~FrameListenerManager()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _listeners.clear();
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::SetCreateDeleteCommandBufferHandler(const FrameCreateDeleteListenerCommandBufferHandler& commandBufferHandler)
    {
        if (_commandBufferHandler)
        {
            KMP_LOG_WARN("overwriting existing command buffer handler");
        }
        else
        {
            KMP_LOG_INFO("setting command buffer handler");
        }

        _commandBufferHandler = commandBufferHandler;
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::PushCommand(FrameListenerCommand&& command) noexcept
    {
        _commandBuffer.emplace_back(std::move(command));
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::AddFrameListener(NonNull<FrameListener*> frameListener)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _listeners.emplace(frameListener->GetPriority(), FrameListenerWrapper(frameListener, true));
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::RemoveFrameListener(NonNull<FrameListener*> frameListener)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto iter = _listeners.begin(); iter != _listeners.end(); iter++)
        {
            if (iter->second.frameListener->GetSID() == frameListener->GetSID())
            {
                _listeners.erase(iter);
                return;
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (const auto& [priority, listener] : _listeners)
        {
            if (listener.isActive)
            {
                listener.frameListener->Update(frameTimestep, mainWindowIsIconified);
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_RenderFrameListeners()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (const auto& [priority, listener] : _listeners)
        {
            if (listener.isActive)
            {
                listener.frameListener->Render();
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_ProcessEventsFrameListeners(Event& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto iter = _listeners.rbegin(); iter != _listeners.rend(); ++iter)
        {
            auto& listener = iter->second;
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

    void FrameListenerManager::_ProcessFrameListenersCommands()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto commandIter = _commandBuffer.begin(); commandIter != _commandBuffer.end();)
        {
            if (commandIter->code == FrameListenerCommandCode::Create || commandIter->code == FrameListenerCommandCode::Delete)
            {
                commandIter++;
                continue;
            }

            auto wrapper = _FindBySid(commandIter->sid);
            if (wrapper == nullptr)
            {
                commandIter = _commandBuffer.erase(commandIter);
                continue;
            }

            wrapper->isActive = (commandIter->code == FrameListenerCommandCode::Activate);
            commandIter++;
        }

        if (_commandBufferHandler)
        {
            _commandBufferHandler(_commandBuffer);
        }

        _commandBuffer.clear();
    }
    //--------------------------------------------------------------------------

    Nullable<FrameListenerManager::FrameListenerWrapper*> FrameListenerManager::_FindBySid(Utils::StringID sid)
    {
        for (auto& [priority, wrapper] : _listeners)
        {
            if (wrapper.frameListener->GetSID() == sid)
            {
                return &wrapper;
            }
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------
}
