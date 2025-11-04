#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Utils/string_id.h"

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
        const Utils::StringID frameListenerSidSource;
        const Utils::StringID frameListenerSidDestination;
    };
    static_assert(std::is_move_constructible_v<FrameListenerCommand> == true);

    using FrameListenerCommandBuffer = Vector<FrameListenerCommand>;
    //--------------------------------------------------------------------------
}