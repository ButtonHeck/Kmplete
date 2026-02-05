#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>

#include <queue>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct PhysicalDeviceImplementationInfo
        {
            KMP_LOG_CLASSNAME(PhysicalDeviceImplementationInfo)

        public:
            UInt32 graphicsFamilyIndex;
            UInt32 presentFamilyIndex;

            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            Vector<VkSurfaceFormatKHR> surfaceFormats;
            Vector<VkPresentModeKHR> presentModes;

            VkPhysicalDeviceMemoryProperties memoryProperties;
            VkPhysicalDeviceProperties deviceProperties;
            VkSampleCountFlags sampleCountsMask;
            std::priority_queue<VkSampleCountFlagBits> supportedSampleCounts;
            VkFormat defaultDepthFormat;
            VkSurfaceFormatKHR surfaceFormat;

            KMP_NODISCARD KMP_API VkSampleCountFlagBits MaximumSupportedSampleCount() const;
            KMP_NODISCARD KMP_API UInt32 FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const;
        };
        //--------------------------------------------------------------------------
    }
}