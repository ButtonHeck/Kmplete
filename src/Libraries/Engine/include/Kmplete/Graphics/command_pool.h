#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class CommandPool
        {
            KMP_DISABLE_COPY_MOVE(CommandPool)

        public:
            CommandPool() = default;
            virtual ~CommandPool() = default;
        };
        //--------------------------------------------------------------------------
    }
}