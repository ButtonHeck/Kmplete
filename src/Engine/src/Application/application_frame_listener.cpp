#include "Kmplete/Application/application_frame_listener.h"


namespace Kmplete
{
    ApplicationFrameListener::ApplicationFrameListener(const String& name) noexcept
        : _name(name)
        , _active(true)
    {}
    //--------------------------------------------------------------------------

    void ApplicationFrameListener::SetActive(bool active) noexcept
    {
        _active = active;
    }
    //--------------------------------------------------------------------------

    bool ApplicationFrameListener::IsActive() const noexcept
    {
        return _active;
    }
    //--------------------------------------------------------------------------
}