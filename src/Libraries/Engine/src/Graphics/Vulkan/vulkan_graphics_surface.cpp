#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
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
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create Vulkan surface: {}", resultDescription);
                throw std::runtime_error(String("VulkanGraphicsSurface: failed to create Vulkan surface: ").append(resultDescription));
            }
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