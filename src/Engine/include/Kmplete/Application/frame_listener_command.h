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
        Create,
        Delete
    };
    //--------------------------------------------------------------------------


    struct FrameListenerCommand
    {
        FrameListenerCommandCode command;
        Utils::StringID sid;
    };
    static_assert(std::is_move_constructible_v<FrameListenerCommand> == true);

    using FrameListenerCommandBuffer = Vector<FrameListenerCommand>;
    //--------------------------------------------------------------------------
}