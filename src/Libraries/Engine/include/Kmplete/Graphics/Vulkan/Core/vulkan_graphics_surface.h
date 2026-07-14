#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_surface.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //! Simple Vulkan API graphics surface object wrapper
        class VulkanGraphicsSurface : public GraphicsSurface
        {
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsSurface)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanGraphicsSurface(Window& window, VkInstance instance);
            KMP_API ~VulkanGraphicsSurface();

            KMP_NODISCARD KMP_API VkSurfaceKHR GetVkSurface() const noexcept;

        private:
            VkInstance _instance;
            VkSurfaceKHR _surface;
        };
        //--------------------------------------------------------------------------
    }
}