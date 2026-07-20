#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Application/frame_listener_command.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/event_dispatcher.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class FrameListenerManager;


    //! Base class for objects that encapsulate logic flow for different stages of the main loop for dedicated parts of the application,
    //! such stages include process events, update and render. Synonim for "ApplicationLayer" in other engines.
    //! Each of these listeners has its own priority and identificator that is used in FrameListenerManager.
    //! Interaction with other listeners and the application is indirect and is implemented by sending commands.
    //! Although a listener can (de)activate and even delete itself, actual state changing is managed outside (from the derived classes pov), 
    //! a listener just pushes appropriate commands. This is done to prevent listener self-destruction within its own update and enforce that
    //! if the update stage had taken place than the rendering stage would also be processed during a frame.
    //! Each frame listener contains its own EventDispatcher instance that by default just dispatches incoming events.
    //! Derived classes may add its own logic for (de)activation.
    //! @see FrameListenerManager
    //! @see FrameListenerCommand
    //! @see Events::EventDispatcher
    class KMP_API FrameListener
    {
        KMP_DISABLE_COPY_MOVE(FrameListener)
        KMP_LOG_CLASSNAME(FrameListener)

    public:
        FrameListener(FrameListenerManager& frameListenerManager, const StringID& sid, UInt8 priority);
        virtual ~FrameListener();

        KMP_NODISCARD StringID GetSID() const noexcept;
        KMP_NODISCARD UInt8 GetPriority() const noexcept;

        KMP_NODISCARD bool IsActive() const noexcept;
        void SetActive(bool active);

        virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        virtual void Render() {}
        virtual void OnEvent(Events::Event& event);
        virtual void OnActivated() {}
        virtual void OnDeactivated() {}

        void PushCommand(FrameListenerCommand&& command) noexcept;

    protected:
        const StringID _sid;
        const UInt8 _priority;
        Events::EventDispatcher _eventDispatcher;

    private:
        friend class FrameListenerManager;

        void _SetActive(bool active);

    private:
        FrameListenerManager& _frameListenerManager;
        bool _active;
    };
    //--------------------------------------------------------------------------
}