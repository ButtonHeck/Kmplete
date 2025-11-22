#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Application/frame_listener_command.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class FrameListenerManager;


    class FrameListener
    {
        KMP_DISABLE_COPY_MOVE(FrameListener)

    public:
        KMP_API FrameListener(FrameListenerManager& frameListenerManager, const Utils::StringID& sid, UInt8 priority) noexcept;
        KMP_API ~FrameListener();

        KMP_NODISCARD KMP_API Utils::StringID GetSID() const noexcept;
        KMP_NODISCARD KMP_API UInt8 GetPriority() const noexcept;

        KMP_NODISCARD KMP_API bool IsActive() const noexcept;
        KMP_API void SetActive(bool active);

        KMP_API virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        KMP_API virtual void Render() {}
        KMP_API virtual void OnEvent(Event&) {}

        KMP_API void PushCommand(FrameListenerCommand&& command) noexcept;

    protected:
        const Utils::StringID _sid;
        const UInt8 _priority;

    private:
        friend class FrameListenerManager;

        FrameListenerManager& _frameListenerManager;
        bool _attached;
        bool _active;
    };
    //--------------------------------------------------------------------------
}