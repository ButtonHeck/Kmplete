#include "Kmplete/Application/frame_listener_manager.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/assertion.h"


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

    bool FrameListenerManager::AddFrameListener(NonNull<FrameListener*> frameListener)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);
        KMP_ASSERT(frameListener);

        const auto listenerWrapper = _FindBySid(frameListener->GetSID());
        if (listenerWrapper != nullptr)
        {
            KMP_LOG_WARN("already contains frame listener with same sid '{}'", frameListener->GetSID());
            return false;
        }
        if (_listeners.contains(frameListener->GetPriority()))
        {
            KMP_LOG_WARN("already contains frame listener '{}' with same priority '{}'", frameListener->GetSID(), frameListener->GetPriority());
            return false;
        }

        const auto [iterator, hasEmplaced] = _listeners.emplace(frameListener->GetPriority(), FrameListenerWrapper(frameListener, true));
        if (hasEmplaced)
        {
            KMP_LOG_INFO("added frame listener '{}' priority {}", frameListener->GetSID(), frameListener->GetPriority());
            return true;
        }
        else
        {
            KMP_LOG_ERROR("failed to add frame listener '{}' priority {}", frameListener->GetSID(), frameListener->GetPriority());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool FrameListenerManager::RemoveFrameListener(NonNull<FrameListener*> frameListener)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);
        KMP_ASSERT(frameListener);

        for (auto iter = _listeners.begin(); iter != _listeners.end(); iter++)
        {
            if (iter->second.frameListener->GetSID() == frameListener->GetSID())
            {
                _listeners.erase(iter);
                KMP_LOG_INFO("removed frame listener '{}' priority {}", frameListener->GetSID(), frameListener->GetPriority());
                return true;
            }
        }

        KMP_LOG_WARN("failed to remove listener '{}' priority {}", frameListener->GetSID(), frameListener->GetPriority());
        return false;
    }
    //--------------------------------------------------------------------------

    size_t FrameListenerManager::FrameListenersCount() const noexcept
    {
        return _listeners.size();
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (const auto& [priority, listenerWrapper] : _listeners)
        {
            if (listenerWrapper.isActive)
            {
                listenerWrapper.frameListener->Update(frameTimestep, mainWindowIsIconified);
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_RenderFrameListeners()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (const auto& [priority, listenerWrapper] : _listeners)
        {
            if (listenerWrapper.isActive)
            {
                listenerWrapper.frameListener->Render();
            }
        }
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_ProcessEventsFrameListeners(Event& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto iter = _listeners.rbegin(); iter != _listeners.rend(); ++iter)
        {
            auto& listenerWrapper = iter->second;
            if (listenerWrapper.isActive)
            {
                if (event.handled)
                {
                    break;
                }

                listenerWrapper.frameListener->OnEvent(event);
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

            auto listenerWrapper = _FindBySid(commandIter->sid);
            if (listenerWrapper == nullptr)
            {
                commandIter = _commandBuffer.erase(commandIter);
                continue;
            }

            listenerWrapper->isActive = (commandIter->code == FrameListenerCommandCode::Activate);
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
        for (auto& [priority, listenerWrapper] : _listeners)
        {
            if (listenerWrapper.frameListener->GetSID() == sid)
            {
                return &listenerWrapper;
            }
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------
}
