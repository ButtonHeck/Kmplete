#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Utils/string_id.h"

#include <ostream>


namespace Kmplete
{
    namespace Events
    {
        using EventTypeID = Utils::StringID;


        enum EventTrait
        {
            None = 0,
            WindowEventTrait        = 1 << 0,
            InputEventTrait         = 1 << 1,
            KeyboardEventTrait      = 1 << 2,
            MouseEventTrait         = 1 << 3,
            MouseButtonEventTrait   = 1 << 4
        };
        //--------------------------------------------------------------------------


    #define EVENT_CLASS_TYPE(eventTypeStr) \
        static constexpr EventTypeID TypeID = Utils::ToStringID(eventTypeStr); \
        static constexpr const char* TypeName = eventTypeStr; \
        KMP_NODISCARD virtual EventTypeID GetTypeID() const noexcept override { return TypeID; } \
        KMP_NODISCARD virtual const char* GetName() const noexcept override { return TypeName; }


        struct Event
        {
            virtual ~Event() = default;

            KMP_NODISCARD virtual EventTypeID GetTypeID() const noexcept = 0;
            KMP_NODISCARD virtual const char* GetName() const noexcept = 0;
            KMP_NODISCARD virtual int GetTraits() const noexcept = 0;
            KMP_NODISCARD virtual String ToString() const
            {
                return GetName();
            }

            KMP_NODISCARD bool HasTrait(EventTrait trait) const noexcept
            {
                return GetTraits() & trait;
            }

            bool handled = false;
        };
        //--------------------------------------------------------------------------

        inline std::ostream& operator<<(std::ostream& os, const Event& event)
        {
            return os << event.ToString();
        }
        //--------------------------------------------------------------------------
    }
}
