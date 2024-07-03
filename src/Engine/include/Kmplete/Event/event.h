#pragma once

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
        WindowEventTrait = 1 << 0,
        InputEventTrait = 1 << 1,
        KeyboardEventTrait = 1 << 2,
        MouseEventTrait = 1 << 3,
        MouseButtonEventTrait = 1 << 4
    };
    //--------------------------------------------------------------------------

#define EVENT_CLASS_TYPE(eventType) \
    static EventType GetStaticType() { return EventType::eventType; } \
    virtual EventType GetType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #eventType; }

    struct Event
    {
        virtual ~Event() = default;

        virtual EventType GetType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetTraits() const = 0;
        virtual std::string ToString() const
        {
            return GetName();
        }

        bool HasTrait(EventTrait trait) const
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
        explicit EventDispatcher(Event& event)
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
