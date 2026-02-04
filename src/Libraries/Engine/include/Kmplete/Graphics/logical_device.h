#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/swapchain.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(LogicalDevice)

        public:
            KMP_API LogicalDevice() noexcept;
            virtual ~LogicalDevice() = default;

        protected:
            UPtr<Swapchain> _swapchain;
        };
        //--------------------------------------------------------------------------
    }
}