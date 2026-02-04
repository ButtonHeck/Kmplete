#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"

#include <vulkan/vulkan.h>


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
        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            Vector<VkSurfaceFormatKHR> surfaceFormats;
            Vector<VkPresentModeKHR> presentModes;
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct HardwareProperties
        {
            VkPhysicalDeviceMemoryProperties memoryProperties;
            VkPhysicalDeviceProperties deviceProperties;
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct PhysicalDeviceProperties
        {
            QueueFamilyIndices queueFamiliesIndices;
            SwapChainSupportDetails swapChainSupportDetails;
            HardwareProperties hardwareProperties;
        };
        //--------------------------------------------------------------------------
    }
}