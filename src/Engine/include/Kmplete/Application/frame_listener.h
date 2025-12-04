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


    class FrameListener
    {
        KMP_DISABLE_COPY_MOVE(FrameListener)
        KMP_LOG_CLASSNAME(FrameListener)

    public:
        KMP_API FrameListener(FrameListenerManager& frameListenerManager, const StringID& sid, UInt8 priority);
        KMP_API virtual ~FrameListener();

        KMP_NODISCARD KMP_API StringID GetSID() const noexcept;
        KMP_NODISCARD KMP_API UInt8 GetPriority() const noexcept;

        KMP_NODISCARD KMP_API bool IsActive() const noexcept;
        KMP_API void SetActive(bool active);

        KMP_API virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        KMP_API virtual void Render() {}
        KMP_API virtual void OnEvent(Events::Event& event);
        KMP_API virtual void OnActivated() {}
        KMP_API virtual void OnDeactivated() {}

        KMP_API void PushCommand(FrameListenerCommand&& command) noexcept;

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