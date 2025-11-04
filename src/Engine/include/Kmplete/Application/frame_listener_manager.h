#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_command.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class FrameListenerManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(FrameListenerManager)

        KMP_API FrameListenerManager() = default;
        KMP_API ~FrameListenerManager();

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
    };
    //--------------------------------------------------------------------------
}