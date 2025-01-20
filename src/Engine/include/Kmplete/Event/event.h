#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <string>
#include <ostream>

namespace Kmplete
{
    enum class EventType
    {
        None = 0,

        WindowCloseEventType,
        WindowResizeEventType,
        WindowMoveEventType,
        WindowFocusEventType,
        WindowIconifyEventType,
        WindowFramebufferResizeEventType,
        WindowFramebufferRefreshEventType,

        KeyPressEventType,
        KeyReleaseEventType,
        KeyCharEventType,

        MouseButtonPressEventType,
        MouseButtonReleaseEventType,
        MouseMoveEventType,
        MouseScrollEventType
    };
    //--------------------------------------------------------------------------

    enum EventTrait
    {
        None = 0,
        WindowEventTrait =      1 << 0,
        InputEventTrait =       1 << 1,
        KeyboardEventTrait =    1 << 2,
        MouseEventTrait =       1 << 3,
        MouseButtonEventTrait = 1 << 4
    };
    //--------------------------------------------------------------------------

#define EVENT_CLASS_TYPE(eventType) \
    KMP_NODISCARD static EventType GetStaticType() KMP_NOEXCEPT { return EventType::eventType; } \
    KMP_NODISCARD virtual EventType GetType() const KMP_NOEXCEPT override { return GetStaticType(); } \
    KMP_NODISCARD virtual const char* GetName() const KMP_NOEXCEPT override { return #eventType; }

    struct Event
    {
        virtual ~Event() = default;

        KMP_NODISCARD virtual EventType GetType() const KMP_NOEXCEPT = 0;
        KMP_NODISCARD virtual const char* GetName() const KMP_NOEXCEPT = 0;
        KMP_NODISCARD virtual int GetTraits() const KMP_NOEXCEPT = 0;
        KMP_NODISCARD virtual std::string ToString() const
        {
            return GetName();
        }

        KMP_NODISCARD bool HasTrait(EventTrait trait) const KMP_NOEXCEPT
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


    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event) KMP_NOEXCEPT
            : _event(event)
        {}

        template<typename Evt, typename Fn>
        bool Dispatch(const Fn& func)
        {
            if (_event.GetType() == Evt::GetStaticType())
            {
                _event.handled |= func(static_cast<Evt&>(_event));
                return true;
            }

            return false;
        }

    private:
        Event& _event;
    };
    //--------------------------------------------------------------------------
}
