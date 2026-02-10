#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/command_pool.h"
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

            KMP_API virtual void CreateSwapchain() = 0;
            KMP_API virtual void DeleteSwapchain() = 0;
            KMP_API virtual void RecreateSwapchain() = 0;

        protected:
            UPtr<CommandPool> _commandPool;
            UPtr<Swapchain> _swapchain;
        };
        //--------------------------------------------------------------------------
    }
}