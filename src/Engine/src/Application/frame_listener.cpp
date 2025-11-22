#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_manager.h"


namespace Kmplete
{
    FrameListener::FrameListener(FrameListenerManager& frameListenerManager, const Utils::StringID& sid, UInt8 priority) noexcept
        : _sid(sid)
        , _frameListenerManager(frameListenerManager)
        , _priority(priority)
        , _attached(false)
        , _active(true)
    {
        _attached = _frameListenerManager.AddFrameListener(this);
    }
    //--------------------------------------------------------------------------

    FrameListener::~FrameListener()
    {
        if (_attached)
        {
            _frameListenerManager.RemoveFrameListener(this);
        }
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
        PushCommand(FrameListenerCommand{ .code = active ? FrameListenerCommandCode::Activate : FrameListenerCommandCode::Deactivate, .sid = _sid });
    }
    //--------------------------------------------------------------------------

    void FrameListener::PushCommand(FrameListenerCommand&& command) noexcept
    {
        _frameListenerManager.PushCommand(std::move(command));
    }
    //--------------------------------------------------------------------------
}