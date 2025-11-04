#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class FrameListenerManager;

    class FrameListener
    {
    public:
        KMP_API FrameListener(FrameListenerManager& frameListenerManager, const String& name) noexcept;
        virtual ~FrameListener() = default;

        KMP_API virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        KMP_API virtual void Render() {}
        KMP_API virtual void OnEvent(Event&) {}

    private:
        FrameListenerManager& _frameListenerManager;
        const String _name;
    };
    //--------------------------------------------------------------------------
}