#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_manager.h"


namespace Kmplete
{
    FrameListener::FrameListener(FrameListenerManager& frameListenerManager, const Utils::StringID& sid, UInt8 priority) noexcept
        : _sid(sid)
        , _frameListenerManager(frameListenerManager)
        , _priority(priority)
    {
        _frameListenerManager.AddFrameListener(this);
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

    void FrameListener::PushCommand(FrameListenerCommand&& command) noexcept
    {
        _frameListenerManager.PushCommand(std::move(command));
    }
    //--------------------------------------------------------------------------
}