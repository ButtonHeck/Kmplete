#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Window/window.h"


namespace Kmplete
{
    namespace Events
    {
        //TODO: comments
        struct EditorFullscreenEvent : public Event
        {
            EVENT_CLASS_TYPE("EditorFullscreenEvent")

            explicit EditorFullscreenEvent(Window::ScreenMode mode) noexcept
                : screenMode(mode)
            {}

            Window::ScreenMode screenMode;
        };
        //--------------------------------------------------------------------------
    }
}