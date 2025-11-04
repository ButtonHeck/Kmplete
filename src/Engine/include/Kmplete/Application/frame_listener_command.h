#pragma once

#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    enum class FrameListenerCommandCode
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
        const FrameListenerCommandCode command;
        const String frameSourceName;
        const String frameDestinationName;
    };
    //--------------------------------------------------------------------------
}