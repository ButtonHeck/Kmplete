#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct VulkanContext
        {
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
        };
        //--------------------------------------------------------------------------
    }
}