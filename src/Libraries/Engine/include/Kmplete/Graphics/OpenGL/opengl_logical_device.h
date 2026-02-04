#pragma once

#include "Kmplete/Graphics/logical_device.h"
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

            KMP_API void CreateSwapchain() override;
            KMP_API void DeleteSwapchain() override;
            KMP_API void RecreateSwapchain() override;
        };
        //--------------------------------------------------------------------------
    }
}