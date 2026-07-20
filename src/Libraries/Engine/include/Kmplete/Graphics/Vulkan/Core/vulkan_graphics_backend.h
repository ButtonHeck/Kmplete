#pragma once

#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;
    class SettingsDocument;


    namespace Graphics
    {
        //! Vulkan API backend class implementation. Responsible for initializing
        //! top-level Vulkan objects such as instance, physical device, surface,
        //! tracking current buffer index that is used by dependent objects.
        class KMP_API VulkanGraphicsBackend : public GraphicsBackend
        {
            KMP_LOG_CLASSNAME(VulkanGraphicsBackend)
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsBackend)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit VulkanGraphicsBackend(Window& window);
            ~VulkanGraphicsBackend();

            KMP_NODISCARD const GraphicsSurface& GetGraphicsSurface() const noexcept override;
            KMP_NODISCARD const VulkanPhysicalDevice& GetPhysicalDevice() const noexcept override;
            KMP_NODISCARD VulkanPhysicalDevice& GetPhysicalDevice() noexcept override;

            void StartFrame(float frameTimestep) override;
            void EndFrame() override;
            void HandleWindowResize() override;

            KMP_NODISCARD Nullable<Texture*> CreateTexture(const Image& image) override;

            KMP_NODISCARD UInt32 GetMultisampling() const override;
            void SetMultisampling(UInt32 samples) override;

            KMP_NODISCARD bool IsVSync() const override;
            void SetVSync(bool vSync) override;

            void SaveSettings(SettingsDocument& settings) const override;
            void LoadSettings(SettingsDocument& settings) override;

            KMP_NODISCARD VkInstance GetVkInstance() const noexcept;
            KMP_NODISCARD UInt32 GetCurrentBufferIndex() const noexcept;

        private:
            void _Initialize();
            void _Finalize();

            KMP_NODISCARD VkApplicationInfo _CreateApplicationInfo() const;
            KMP_NODISCARD Vector<const char*> _GetRequiredExtensionsNames() const;
            KMP_NODISCARD VkInstanceCreateInfo _CreateInstanceCreateInfo(const VkApplicationInfo& applicationInfo, Vector<const char*>& extensionsNames) const;

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
            KMP_NODISCARD bool _CheckValidationLayerSupport() const;
            void _AttachDebugMessengerInfo(VkInstanceCreateInfo& instanceCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& debugMessengerCreateInfo) const;
            void _PrintAvailableExtensions() const;
            void _InitializeDebugMessenger();
#endif

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