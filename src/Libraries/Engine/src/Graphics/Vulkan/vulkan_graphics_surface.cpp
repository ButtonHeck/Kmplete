#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
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
        VulkanGraphicsSurface::VulkanGraphicsSurface(Window& window, const VkInstance& instance)
            : GraphicsSurface(window)
            , _instance(instance)
        {
            GLFWwindow* windowImpl = reinterpret_cast<GLFWwindow*>(window.GetImplPointer());
            if (glfwCreateWindowSurface(_instance, windowImpl, nullptr, &_surface) != VK_SUCCESS)
            {
                KMP_LOG_CRITICAL("failed to create Vulkan surface");
                throw std::runtime_error("VulkanGraphicsSurface: failed to create Vulkan surface");
            }
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsSurface::~VulkanGraphicsSurface()
        {
            if (_surface)
            {
                vkDestroySurfaceKHR(_instance, _surface, nullptr);
            }
        }
        //--------------------------------------------------------------------------
    }
}