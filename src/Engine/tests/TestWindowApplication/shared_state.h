#pragma once

#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    static constexpr auto frame1Mask = 0b0001;
    static constexpr auto frame2Mask = 0b0010;
    static constexpr auto frame3Mask = 0b0100;
    static constexpr auto frame4Mask = 0b1000;


    struct SharedState
    {
        int existenceMask = 0;
        int updateMask = 0;
        String updateMaskString = "";
        String eventProcessingString = "";
        int eventAcceptMask = 0;
    };
    //--------------------------------------------------------------------------
}