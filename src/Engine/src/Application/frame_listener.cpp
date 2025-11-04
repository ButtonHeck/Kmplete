#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_manager.h"


namespace Kmplete
{
    FrameListener::FrameListener(FrameListenerManager& frameListenerManager, const String& name) noexcept
        : _frameListenerManager(frameListenerManager)
        , _name(name)
    {}
    //--------------------------------------------------------------------------
}