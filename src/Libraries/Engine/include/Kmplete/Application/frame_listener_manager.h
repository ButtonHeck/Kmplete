#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_command.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! Manager for FrameListener objects, tightly coupled with WindowApplication, responsible for: storing/adding/removing frame listeners, 
    //! handling their intercommunications between each other and the application (via processing commands), ordering listeners' logic stages processing.
    //! This manager is not responsible for controlling lifetime of contained listeners - this should be done in the client application code mainly because
    //! client frame listeners are logically tied to their client application and when the client application destroys all its frame listeners should
    //! already be finalized and destroyed (additionally client frame listeners may contain some additional startup/shutdown behaviour).
    //! Command processing is split into two parts:
    //! 1) (de)activation is done here just by setting listeners' active flag (Activate/Deactivate command codes)
    //! 2) creation and deletion of frame listeners delegated to the outer handler function that should be set by the client application (Create/Delete command codes)
    //! @see FrameListener
    //! @see WindowApplication
    class FrameListenerManager
    {
        KMP_LOG_CLASSNAME(FrameListenerManager)
        KMP_DISABLE_COPY_MOVE(FrameListenerManager)

    public:
        using FrameCreateDeleteListenerCommandBufferHandler = Function<void(const FrameListenerCommandBuffer&)>;

    public:
        KMP_API FrameListenerManager() = default;
        KMP_API ~FrameListenerManager();

        KMP_API void SetCreateDeleteCommandBufferHandler(const FrameCreateDeleteListenerCommandBufferHandler& commandBufferHandler);
        KMP_API void PushCommand(FrameListenerCommand&& command) noexcept;

        KMP_API bool AddFrameListener(NonNull<FrameListener*> frameListener);
        KMP_API bool RemoveFrameListener(NonNull<FrameListener*> frameListener);

        KMP_NODISCARD KMP_API size_t FrameListenersCount() const noexcept;

    private:
        friend class WindowApplication;

        void _UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified);
        void _RenderFrameListeners();
        void _DispatchEventToFrameListeners(Events::Event& event);
        void _DispatchQueuedEventsToFrameListeners();
        void _ProcessFrameListenersCommands();

        Nullable<FrameListener*> _FindFrameListener(StringID sid);

    private:
        Map<UInt8, Nullable<FrameListener*>> _listeners;
        FrameListenerCommandBuffer _commandBuffer;
        FrameCreateDeleteListenerCommandBufferHandler _commandBufferHandler;
    };
    //--------------------------------------------------------------------------
}