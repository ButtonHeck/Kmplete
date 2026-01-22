#pragma once

#include "Kmplete/Event/event.h"


namespace Kmplete
{
    namespace Events
    {
        struct CustomEvent : public Event
        {
            EVENT_CLASS_TYPE("CustomEvent")

            CustomEvent() = default;
        };
        //--------------------------------------------------------------------------
    }
}