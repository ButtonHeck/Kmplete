#pragma once

#include "Kmplete/Graphics/framebuffer.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanFramebuffer : public Framebuffer
        {
            KMP_DISABLE_COPY_MOVE(VulkanFramebuffer)
            KMP_LOG_CLASSNAME(VulkanFramebuffer)

        public:
            KMP_API VulkanFramebuffer();
            KMP_API ~VulkanFramebuffer();

        private:
        };
        //--------------------------------------------------------------------------
    }
}