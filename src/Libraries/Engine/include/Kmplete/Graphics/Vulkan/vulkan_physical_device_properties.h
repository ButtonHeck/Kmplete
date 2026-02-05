#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"

#include <vulkan/vulkan.h>

#include <queue>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct QueueFamilyIndices
        {
            Optional<UInt32> graphicsFamilyIndex;
            Optional<UInt32> presentFamilyIndex;

            inline bool IsValid() const noexcept
            {
                return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
            }
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct SurfaceAndPresentModeProperties
        {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            Vector<VkSurfaceFormatKHR> surfaceFormats;
            Vector<VkPresentModeKHR> presentModes;
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct PhysicalDeviceProperties
        {
            VkPhysicalDeviceMemoryProperties memoryProperties;
            VkPhysicalDeviceProperties deviceProperties;
            VkSampleCountFlags sampleCountsMask;
            std::priority_queue<VkSampleCountFlagBits> supportedSampleCounts;

            inline VkSampleCountFlagBits MaximumSupportedSampleCount() const noexcept
            {
                return supportedSampleCounts.top();
            }
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct PhysicalDeviceImplementationInfo
        {
            QueueFamilyIndices queueFamiliesIndices;
            SurfaceAndPresentModeProperties surfaceAndPresentModeProperties;
            PhysicalDeviceProperties physicalDeviceProperties;
        };
        //--------------------------------------------------------------------------
    }
}