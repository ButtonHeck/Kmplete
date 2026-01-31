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
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsSurface)
            KMP_LOG_CLASSNAME(VulkanGraphicsSurface)

        public:
            KMP_API VulkanGraphicsSurface(Window& window, const VkInstance& instance);
            KMP_API ~VulkanGraphicsSurface();

            KMP_NODISCARD KMP_API const VkSurfaceKHR& GetImplSurface() const noexcept;

        private:
            const VkInstance& _instance;
            VkSurfaceKHR _surface;
        };
        //--------------------------------------------------------------------------
    }
}