#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class FrameListener
    {
    public:
        KMP_API explicit FrameListener(const String& name) noexcept;
        virtual ~FrameListener() = default;

        KMP_API virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        KMP_API virtual void Render() {}
        KMP_API virtual void OnEvent(Event&) {}

    private:
        const String _name;
    };
    //--------------------------------------------------------------------------

    struct FrameListenerWrapper
    {
        Nullable<FrameListener*> frameListener;
        bool isActive;
    };
    //--------------------------------------------------------------------------

    enum class FrameCommandCode
    {
        Activate,
        Deactivate,
        Replace,
        Delete,
        AddAfter,
        AddBefore
    };
    //--------------------------------------------------------------------------

    struct FrameListenerCommand
    {
        const FrameCommandCode command;
        const String frameSourceName;
        const String frameDestinationName;
    };
    //--------------------------------------------------------------------------
}