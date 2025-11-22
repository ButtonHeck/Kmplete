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

        struct FrameListenerWrapper
        {
            Nullable<FrameListener*> frameListener;
            bool isActive;
        };

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
        void _ProcessEventsFrameListeners(Event& event);
        void _ProcessFrameListenersCommands();

        Nullable<FrameListenerWrapper*> _FindBySid(Utils::StringID sid);

    private:
        Map<UInt8, FrameListenerWrapper> _listeners;
        FrameListenerCommandBuffer _commandBuffer;
        FrameCreateDeleteListenerCommandBufferHandler _commandBufferHandler;
    };
    //--------------------------------------------------------------------------
}