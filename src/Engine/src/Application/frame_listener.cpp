#include "Kmplete/Application/frame_listener.h"


namespace Kmplete
{
    FrameListener::FrameListener(const String& name) noexcept
        : _name(name)
        , _active(true)
    {}
    //--------------------------------------------------------------------------

    void FrameListener::SetActive(bool active) noexcept
    {
        _active = active;
    }
    //--------------------------------------------------------------------------

    bool FrameListener::IsActive() const noexcept
    {
        return _active;
    }
    //--------------------------------------------------------------------------
}