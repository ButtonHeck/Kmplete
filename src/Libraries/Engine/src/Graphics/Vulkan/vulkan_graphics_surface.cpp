#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"

#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanGraphicsSurface::VulkanGraphicsSurface(Window& window, VkInstance instance)
            : GraphicsSurface(window)
            , _instance(instance)
        {
            GLFWwindow* windowImpl = reinterpret_cast<GLFWwindow*>(window.GetImplPointer().get());
            const auto result = glfwCreateWindowSurface(_instance, windowImpl, nullptr, &_surface);
            VulkanUtils::CheckResult(result, "VulkanGraphicsSurface: failed to create Vulkan surface");
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsSurface::~VulkanGraphicsSurface()
        {
            vkDestroySurfaceKHR(_instance, _surface, nullptr);
        }
        //--------------------------------------------------------------------------

        VkSurfaceKHR VulkanGraphicsSurface::GetVkSurface() const noexcept
        {
            return _surface;
        }
        //--------------------------------------------------------------------------
    }
}