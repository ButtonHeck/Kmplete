#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct VulkanContext
        {
            KMP_LOG_CLASSNAME(VulkanContext)

        public:
            VkInstance instance;
            VkPhysicalDevice physicalDevice;

            UInt32 graphicsFamilyIndex{};
            UInt32 presentFamilyIndex{};

            VkSurfaceCapabilitiesKHR surfaceCapabilities{};
            Vector<VkSurfaceFormatKHR> surfaceFormats{};
            Vector<VkPresentModeKHR> presentModes{};

            VkPhysicalDeviceMemoryProperties memoryProperties{};
            VkPhysicalDeviceProperties deviceProperties{};

            VkSampleCountFlags sampleCountsMask{};
            PriorityQueue<VkSampleCountFlagBits> supportedSampleCounts{};

            VkFormat defaultDepthFormat{};
            VkSurfaceFormatKHR surfaceFormat{};

        public:
            KMP_API void Populate(VkInstance vkInstance, VkPhysicalDevice physDevice, VkFormat depthFormat, UInt32 graphicsIndex, UInt32 presentIndex,
                                  const VkSurfaceCapabilitiesKHR& surfCapabilities, Vector<VkSurfaceFormatKHR>&& surfFormats, Vector<VkPresentModeKHR>&& presentModesParam);

        private:
            KMP_NODISCARD VkSurfaceFormatKHR _FindSurfaceFormat() const;
        };
        //--------------------------------------------------------------------------
    }
}