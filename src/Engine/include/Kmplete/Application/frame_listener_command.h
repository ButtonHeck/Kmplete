#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Base/string_id.h"


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
        FrameListenerCommandCode code;
        StringID sid;
    };
    static_assert(IsMoveConstructible<FrameListenerCommand>::value);

    using FrameListenerCommandBuffer = Vector<FrameListenerCommand>;
    //--------------------------------------------------------------------------
}