#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_surface.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class VulkanGraphicsSurface : public GraphicsSurface
        {
            KMP_LOG_CLASSNAME(VulkanGraphicsSurface)

        public:
            KMP_API VulkanGraphicsSurface(Window& window, const VkInstance& instance);
            KMP_API ~VulkanGraphicsSurface();

        private:
            const VkInstance& _instance;
            VkSurfaceKHR _surface;
        };
        //--------------------------------------------------------------------------
    }
}