#pragma once

#include "Kmplete/Event/event.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    //! Definitions of graphics events supported by engine

    namespace Events
    {
        static constexpr auto MultisamplingChangeEventTypeStr = "MultisamplingChangeEvent";

        static constexpr auto MultisamplingChangeEventTypeID = ToStringID(MultisamplingChangeEventTypeStr);


        struct GraphicsEvent : public Event
        {
        protected:
            GraphicsEvent() = default;
        };
        //--------------------------------------------------------------------------


        struct MultisamplingChangeEvent : public GraphicsEvent
        {
            EVENT_CLASS_TYPE(MultisamplingChangeEventTypeStr)

            explicit MultisamplingChangeEvent(UInt32 samples) noexcept
                : _msaaSamples(samples)
            {}

            KMP_NODISCARD UInt32 GetSamples() const noexcept
            {
                return _msaaSamples;
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate("MultisamplingChangeEvent: ", _msaaSamples);
            }

        private:
            const UInt32 _msaaSamples;
        };
        //--------------------------------------------------------------------------
    }
}