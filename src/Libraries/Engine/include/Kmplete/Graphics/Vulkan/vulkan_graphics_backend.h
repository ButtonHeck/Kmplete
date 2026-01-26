#pragma once

#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    //TODO: comments
    class VulkanGraphicsBackend : public GraphicsBackend
    {
        KMP_LOG_CLASSNAME(VulkanGraphicsBackend)
        KMP_DISABLE_COPY_MOVE(VulkanGraphicsBackend)

    public:
        KMP_API VulkanGraphicsBackend();
        KMP_API ~VulkanGraphicsBackend();

    private:
        void _Initialize();
        void _Finalize();

        KMP_NODISCARD bool _CheckValidationLayerSupport() const;
        KMP_NODISCARD VkApplicationInfo _CreateApplicationInfo() const;
        KMP_NODISCARD Vector<const char*> _GetRequiredExtensionsNames() const;
        KMP_NODISCARD VkInstanceCreateInfo _CreateInstanceCreateInfo(const VkApplicationInfo& applicationInfo, Vector<const char*>& extensionsNames) const;
        void _AttachDebugMessenger(VkInstanceCreateInfo& instanceCreateInfo) const;
        void _PrintAvailableExtensions() const;

    private:
        VkInstance _instance;
    };
    //--------------------------------------------------------------------------
}