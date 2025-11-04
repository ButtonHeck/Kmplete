#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_manager.h"


namespace Kmplete
{
    FrameListener::FrameListener(FrameListenerManager& frameListenerManager, const Utils::StringID& sid) noexcept
        : _sid(sid)
        , _frameListenerManager(frameListenerManager)
    {}
    //--------------------------------------------------------------------------

    Utils::StringID FrameListener::GetSID() const noexcept
    {
        return _sid;
    }
    //--------------------------------------------------------------------------

    void FrameListener::PushCommand(FrameListenerCommand&& command) noexcept
    {
        _frameListenerManager.PushCommand(std::move(command));
    }
    //--------------------------------------------------------------------------
}