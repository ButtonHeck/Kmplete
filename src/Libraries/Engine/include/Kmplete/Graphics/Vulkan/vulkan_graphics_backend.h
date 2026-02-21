#pragma once

#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class VulkanGraphicsBackend : public GraphicsBackend
        {
            KMP_LOG_CLASSNAME(VulkanGraphicsBackend)
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsBackend)

        public:
            KMP_API explicit VulkanGraphicsBackend(Window& window);
            KMP_API ~VulkanGraphicsBackend();

            KMP_NODISCARD KMP_API const GraphicsSurface& GetGraphicsSurface() const noexcept override;
            KMP_NODISCARD KMP_API const PhysicalDevice& GetPhysicalDevice() const noexcept override;

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;
            KMP_API void HandleWindowResize() override;

            KMP_NODISCARD KMP_API VkInstance GetVkInstance() const noexcept;

        private:
            void _Initialize();
            void _Finalize();

            KMP_NODISCARD bool _CheckValidationLayerSupport() const;
            KMP_NODISCARD VkApplicationInfo _CreateApplicationInfo() const;
            KMP_NODISCARD Vector<const char*> _GetRequiredExtensionsNames() const;
            KMP_NODISCARD VkInstanceCreateInfo _CreateInstanceCreateInfo(const VkApplicationInfo& applicationInfo, Vector<const char*>& extensionsNames) const;
            void _AttachDebugMessengerInfo(VkInstanceCreateInfo& instanceCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& debugMessengerCreateInfo) const;
            void _PrintAvailableExtensions() const;
            void _InitializeDebugMessenger();

        private:
            VkInstance _instance;
            UPtr<VulkanGraphicsSurface> _surface;
            UPtr<VulkanPhysicalDevice> _physicalDevice;
            VkDebugUtilsMessengerEXT _debugMessenger;
            UInt32 _currentBufferIndex;
        };
        //--------------------------------------------------------------------------
    }
}