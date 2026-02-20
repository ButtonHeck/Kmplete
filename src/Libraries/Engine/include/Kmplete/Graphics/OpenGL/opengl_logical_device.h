#pragma once

#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/OpenGL/opengl_command_pool.h"
#include "Kmplete/Graphics/OpenGL/opengl_swapchain.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OpenGLLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(OpenGLLogicalDevice)

        public:
            KMP_API OpenGLLogicalDevice() noexcept;
            KMP_API ~OpenGLLogicalDevice();

            KMP_NODISCARD KMP_API const CommandPool& GetCommandPool() const noexcept override;
            KMP_NODISCARD KMP_API const Swapchain& GetSwapchain() const noexcept override;

            KMP_API void CreateSwapchain() override;
            KMP_API void DeleteSwapchain() override;
            KMP_API void RecreateSwapchain() override;

        private:
            UPtr<OpenGLCommandPool> _commandPool;
            UPtr<OpenGLSwapchain> _swapchain;
        };
        //--------------------------------------------------------------------------
    }
}