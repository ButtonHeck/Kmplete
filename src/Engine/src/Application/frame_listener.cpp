#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_manager.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    FrameListener::FrameListener(FrameListenerManager& frameListenerManager, const Utils::StringID& sid, UInt8 priority)
        : _sid(sid)
        , _frameListenerManager(frameListenerManager)
        , _priority(priority)
        , _active(true)
    {
        if (!_frameListenerManager.AddFrameListener(this))
        {
            KMP_LOG_ERROR("failed to attach frame listener '{}' to manager", sid);
            throw std::runtime_error("FrameListener: failed to attach to frame listener manager");
        }
    }
    //--------------------------------------------------------------------------

    FrameListener::~FrameListener()
    {
        _frameListenerManager.RemoveFrameListener(this);
    }
    //--------------------------------------------------------------------------

    Utils::StringID FrameListener::GetSID() const noexcept
    {
        return _sid;
    }
    //--------------------------------------------------------------------------

    UInt8 FrameListener::GetPriority() const noexcept
    {
        return _priority;
    }
    //--------------------------------------------------------------------------

    bool FrameListener::IsActive() const noexcept
    {
        return _active;
    }
    //--------------------------------------------------------------------------

    void FrameListener::SetActive(bool active)
    {
        if ((active && IsActive()) || (!active && !IsActive()))
        {
            return;
        }

        PushCommand(FrameListenerCommand{ .code = active ? FrameListenerCommandCode::Activate : FrameListenerCommandCode::Deactivate, .sid = _sid });
    }
    //--------------------------------------------------------------------------

    void FrameListener::OnEvent(Events::Event& event)
    {
        _eventDispatcher.Dispatch(event);
    }
    //--------------------------------------------------------------------------

    void FrameListener::PushCommand(FrameListenerCommand&& command) noexcept
    {
        _frameListenerManager.PushCommand(std::move(command));
    }
    //--------------------------------------------------------------------------

    void FrameListener::_SetActive(bool active)
    {
        if ((active && IsActive()) || (!active && !IsActive()))
        {
            return;
        }

        _active = active;

        _active ? OnActivated() : OnDeactivated();
    }
    //--------------------------------------------------------------------------
}