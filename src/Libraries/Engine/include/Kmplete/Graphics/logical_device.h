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
            LogicalDevice() = default;
            virtual ~LogicalDevice() = default;

            KMP_API virtual void StartFrame(float /*frameTimestep*/) {};
            KMP_API virtual void EndFrame() {};

            KMP_NODISCARD KMP_API virtual const CommandPool& GetCommandPool() const noexcept = 0;
            KMP_NODISCARD KMP_API virtual const Swapchain& GetSwapchain() const noexcept = 0;

            KMP_API virtual void CreateSwapchain() = 0;
            KMP_API virtual void DeleteSwapchain() = 0;
            KMP_API virtual void RecreateSwapchain() = 0;
        };
        //--------------------------------------------------------------------------
    }
}