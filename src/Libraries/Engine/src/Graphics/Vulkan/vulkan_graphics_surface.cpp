#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif


namespace Kmplete
{
    namespace Graphics
    {
        VulkanGraphicsSurface::VulkanGraphicsSurface(Window& window, VkInstance instance)
            : GraphicsSurface(window)
            , _instance(instance)
            , _surface(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            KMP_ASSERT(_instance);

#if defined (KMP_WINDOW_BACKEND_GLFW)
            GLFWwindow* windowImpl = reinterpret_cast<GLFWwindow*>(window.GetImplPointer().get());
            const auto result = glfwCreateWindowSurface(_instance, windowImpl, nullptr, &_surface);
            VKUtils::CheckResult(result, "VulkanGraphicsSurface: failed to create Vulkan surface");
#endif

            KMP_ASSERT(_surface);
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsSurface::~VulkanGraphicsSurface() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_instance && _surface);

            vkDestroySurfaceKHR(_instance, _surface, nullptr);
        }}
        //--------------------------------------------------------------------------

        VkSurfaceKHR VulkanGraphicsSurface::GetVkSurface() const noexcept
        {
            KMP_ASSERT(_surface);

            return _surface;
        }
        //--------------------------------------------------------------------------
    }
}