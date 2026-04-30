#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace Events
    {
        struct MultisamplingChangeEvent : public Event
        {
            EVENT_CLASS_TYPE("MultisamplingChangeEvent")

            MultisamplingChangeEvent(UInt32 samples)
                : msaaSamples(samples)
            {}

            UInt32 msaaSamples;
        };
        //--------------------------------------------------------------------------
    }
}