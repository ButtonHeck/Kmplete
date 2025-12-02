#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"

#include <ostream>


namespace Kmplete
{
    namespace Events
    {
        using EventTypeID = StringID;


    #define EVENT_CLASS_TYPE(eventTypeStr) \
        static constexpr EventTypeID TypeID = ToStringID(eventTypeStr); \
        static constexpr const char* TypeName = eventTypeStr; \
        KMP_NODISCARD virtual EventTypeID GetTypeID() const noexcept override { return TypeID; } \
        KMP_NODISCARD virtual const char* GetName() const noexcept override { return TypeName; }


        struct Event
        {
            virtual ~Event() = default;

            KMP_NODISCARD virtual EventTypeID GetTypeID() const noexcept = 0;
            KMP_NODISCARD virtual const char* GetName() const noexcept = 0;
            KMP_NODISCARD virtual String ToString() const
            {
                return GetName();
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
