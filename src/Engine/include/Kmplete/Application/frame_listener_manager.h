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
        using FrameCommandBufferHandler = std::function<void()>;

    public:
        KMP_DISABLE_COPY_MOVE(FrameListenerManager)

        KMP_API FrameListenerManager() = default;
        KMP_API ~FrameListenerManager();

        KMP_API void SetCommandBufferHandler(const FrameCommandBufferHandler& commandBufferHandler);
        KMP_API void PushCommand(FrameListenerCommand&& command);

    private:
        friend class WindowApplication;

        struct FrameListenerWrapper
        {
            Nullable<FrameListener*> frameListener;
            bool isActive;
        };
        
        void AddFrameListener(NonNull<FrameListener*> frameListener);

        void UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified);
        void RenderFrameListeners();
        void ProcessEventsFrameListeners(Event& event);
        void ProcessFrameListenersCommands();

    private:
        Vector<FrameListenerWrapper> _listeners;
        Vector<FrameListenerCommand> _commandBuffer;
        FrameCommandBufferHandler _commandBufferHandler;
    };
    //--------------------------------------------------------------------------
}