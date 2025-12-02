#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_command.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Log/log_class_macro.h"

#include <functional>


namespace Kmplete
{
    class FrameListenerManager
    {
        KMP_LOG_CLASSNAME(FrameListenerManager)

    public:
        using FrameCreateDeleteListenerCommandBufferHandler = std::function<void(const FrameListenerCommandBuffer&)>;

    public:
        KMP_DISABLE_COPY_MOVE(FrameListenerManager)

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

        Nullable<FrameListener*> _FindBySid(StringID sid);

    private:
        Map<UInt8, Nullable<FrameListener*>> _listeners;
        FrameListenerCommandBuffer _commandBuffer;
        FrameCreateDeleteListenerCommandBufferHandler _commandBufferHandler;
    };
    //--------------------------------------------------------------------------
}