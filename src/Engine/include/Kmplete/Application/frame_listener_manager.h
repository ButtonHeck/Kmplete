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
        using FrameListenerCommandBufferHandler = std::function<void(const FrameListenerCommandBuffer&)>;

    public:
        KMP_DISABLE_COPY_MOVE(FrameListenerManager)

        KMP_API FrameListenerManager() = default;
        KMP_API ~FrameListenerManager();

        KMP_API void SetCommandBufferHandler(const FrameListenerCommandBufferHandler& commandBufferHandler);
        KMP_API void PushCommand(FrameListenerCommand&& command) noexcept;

    private:
        friend class WindowApplication;

        struct _FrameListenerWrapper
        {
            Nullable<FrameListener*> frameListener;
            bool isActive;
        };
        
        void _AddFrameListener(NonNull<FrameListener*> frameListener);
        void _RemoveFrameListener(NonNull<FrameListener*> frameListener);

        void _UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified);
        void _RenderFrameListeners();
        void _ProcessEventsFrameListeners(Event& event);
        void _ProcessFrameListenersCommands();

    private:
        Vector<_FrameListenerWrapper> _listeners;
        FrameListenerCommandBuffer _commandBuffer;
        FrameListenerCommandBufferHandler _commandBufferHandler;
    };
    //--------------------------------------------------------------------------
}