#pragma once

#include "Kmplete/Base/types_aliases.h"

#include <type_traits>


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
    static_assert(std::is_move_constructible_v<FrameListenerCommand> == true);

    using FrameListenerCommandBuffer = Vector<FrameListenerCommand>;
    //--------------------------------------------------------------------------
}