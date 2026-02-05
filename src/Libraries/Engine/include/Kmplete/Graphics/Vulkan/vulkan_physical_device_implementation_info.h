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
            VkFormat defaultDepthFormat;

            inline VkSampleCountFlagBits MaximumSupportedSampleCount() const noexcept
            {
                return supportedSampleCounts.top();
            }

            inline UInt32 FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const
            {
                for (UInt32 i = 0; i < memoryProperties.memoryTypeCount; i++)
                {
                    if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                    {
                        return i;
                    }
                }

                //KMP_LOG_CRITICAL_FN("failed to find suitable memory type");
                //throw std::runtime_error("PhysicalDeviceProperties: failed to find suitable memory type");

                //TODO: fix
                return 0;
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