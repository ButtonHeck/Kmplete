#include "Kmplete/Application/frame_listener_manager.h"
#include "Kmplete/Event/event_queue.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    FrameListenerManager::~FrameListenerManager() KMP_PROFILING(ProfileLevelAlways)
    {
        _listeners.clear();
    }}
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

    bool FrameListenerManager::AddFrameListener(NonNull<FrameListener*> frameListener) KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(frameListener);

        if (_FindFrameListener(frameListener->GetSID()) != nullptr)
        {
            KMP_LOG_WARN("already contains frame listener with same sid '{}'", frameListener->GetSID());
            return false;
        }
        if (_listeners.contains(frameListener->GetPriority()))
        {
            KMP_LOG_WARN("already contains frame listener '{}' with same priority '{}'", frameListener->GetSID(), frameListener->GetPriority());
            return false;
        }

        const auto [iterator, hasEmplaced] = _listeners.emplace(frameListener->GetPriority(), frameListener);
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
    }}
    //--------------------------------------------------------------------------

    bool FrameListenerManager::RemoveFrameListener(NonNull<FrameListener*> frameListener) KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(frameListener);

        for (auto iter = _listeners.begin(); iter != _listeners.end(); iter++)
        {
            if (iter->second->GetSID() == frameListener->GetSID())
            {
                _listeners.erase(iter);
                KMP_LOG_INFO("removed frame listener '{}' priority {}", frameListener->GetSID(), frameListener->GetPriority());
                return true;
            }
        }

        KMP_LOG_WARN("failed to remove listener '{}' priority {}", frameListener->GetSID(), frameListener->GetPriority());
        return false;
    }}
    //--------------------------------------------------------------------------

    size_t FrameListenerManager::FrameListenersCount() const noexcept
    {
        return _listeners.size();
    }
    //--------------------------------------------------------------------------

    void FrameListenerManager::_UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified) KMP_PROFILING(ProfileLevelImportant)
    {
        for (const auto& [priority, listener] : _listeners)
        {
            if (listener->IsActive())
            {
                listener->Update(frameTimestep, mainWindowIsIconified);
            }
        }
    }}
    //--------------------------------------------------------------------------

    void FrameListenerManager::_RenderFrameListeners() KMP_PROFILING(ProfileLevelImportant)
    {
        for (const auto& [priority, listener] : _listeners)
        {
            if (listener->IsActive())
            {
                listener->Render();
            }
        }
    }}
    //--------------------------------------------------------------------------

    void FrameListenerManager::_DispatchEventToFrameListeners(Events::Event& event) KMP_PROFILING(ProfileLevelImportant)
    {
        for (auto iter = _listeners.rbegin(); iter != _listeners.rend(); ++iter)
        {
            auto& listener = iter->second;
            if (listener->IsActive())
            {
                if (event.handled)
                {
                    break;
                }

                listener->OnEvent(event);
            }
        }
    }}
    //--------------------------------------------------------------------------

    void FrameListenerManager::_DispatchQueuedEventsToFrameListeners() KMP_PROFILING(ProfileLevelImportant)
    {
        auto& eventQueue = Events::EventQueue::Get().GetEvents();
        for (auto eventIter = eventQueue.begin(); eventIter != eventQueue.end();)
        {
            Events::Event& event = *eventIter->get();
            _DispatchEventToFrameListeners(event);
            eventIter = eventQueue.erase(eventIter);
        }
    }}
    //--------------------------------------------------------------------------

    void FrameListenerManager::_ProcessFrameListenersCommands() KMP_PROFILING(ProfileLevelImportant)
    {
        if (_commandBuffer.empty())
        {
            return;
        }

        for (auto commandIter = _commandBuffer.begin(); commandIter != _commandBuffer.end();)
        {
            if (commandIter->code == FrameListenerCommandCode::Create || commandIter->code == FrameListenerCommandCode::Delete)
            {
                commandIter++;
                continue;
            }

            auto listener = _FindFrameListener(commandIter->sid);
            if (listener == nullptr)
            {
                KMP_LOG_WARN("failed to find frame listener '{}'", commandIter->sid);
                commandIter = _commandBuffer.erase(commandIter);
                continue;
            }

            listener->_SetActive(commandIter->code == FrameListenerCommandCode::Activate);
            commandIter++;
        }

        if (_commandBufferHandler)
        {
            _commandBufferHandler(_commandBuffer);
        }

        _commandBuffer.clear();
    }}
    //--------------------------------------------------------------------------

    Nullable<FrameListener*> FrameListenerManager::_FindFrameListener(StringID sid)
    {
        for (auto& [priority, listener] : _listeners)
        {
            if (listener->GetSID() == sid)
            {
                return listener;
            }
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------
}
