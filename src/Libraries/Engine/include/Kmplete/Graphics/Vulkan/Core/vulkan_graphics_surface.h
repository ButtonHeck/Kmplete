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
        class KMP_API VulkanGraphicsSurface : public GraphicsSurface
        {
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsSurface)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanGraphicsSurface(Window& window, VkInstance instance);
            ~VulkanGraphicsSurface();

            KMP_NODISCARD VkSurfaceKHR GetVkSurface() const noexcept;

        private:
            VkInstance _instance;
            VkSurfaceKHR _surface;
        };
        //--------------------------------------------------------------------------
    }
}