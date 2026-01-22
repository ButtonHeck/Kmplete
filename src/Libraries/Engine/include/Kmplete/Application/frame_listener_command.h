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


    //! Description of a single command used for intercommunication between frame listeners and application
    //! @see FrameListener 
    //! @see FrameListenerManager
    struct FrameListenerCommand
    {
        FrameListenerCommandCode code;
        StringID sid;
    };
    static_assert(IsMoveConstructible<FrameListenerCommand>::value);

    using FrameListenerCommandBuffer = Vector<FrameListenerCommand>;
    //--------------------------------------------------------------------------
}