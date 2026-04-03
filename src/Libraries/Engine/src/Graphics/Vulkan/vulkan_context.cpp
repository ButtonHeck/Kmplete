#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        void VulkanContext::Populate(VkInstance vkInstance, VkPhysicalDevice physDevice, VkSurfaceKHR surfaceParam, VkFormat depthFormat, UInt32 graphicsIndex, UInt32 presentIndex,
                                     const VkSurfaceCapabilitiesKHR& surfCapabilities, Vector<VkSurfaceFormatKHR>&& surfFormats, Vector<VkPresentModeKHR>&& presentModesParam)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            this->instance = vkInstance;
            this->physicalDevice = physDevice;
            this->surface = surfaceParam;
            this->graphicsFamilyIndex = graphicsIndex;
            this->presentFamilyIndex = presentIndex;
            this->surfaceCapabilities = surfCapabilities;
            this->surfaceFormats = std::move(surfFormats);
            this->presentModes = std::move(presentModesParam);

            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
            vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

            const auto& properties = deviceProperties;
            sampleCountsMask = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
            const auto samplesCountMask = sampleCountsMask;
            if (samplesCountMask & VK_SAMPLE_COUNT_64_BIT)
                supportedSampleCounts.push(VK_SAMPLE_COUNT_64_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_32_BIT)
                supportedSampleCounts.push(VK_SAMPLE_COUNT_32_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_16_BIT)
                supportedSampleCounts.push(VK_SAMPLE_COUNT_16_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_8_BIT)
                supportedSampleCounts.push(VK_SAMPLE_COUNT_8_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_4_BIT)
                supportedSampleCounts.push(VK_SAMPLE_COUNT_4_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_2_BIT)
                supportedSampleCounts.push(VK_SAMPLE_COUNT_2_BIT);
            else
                supportedSampleCounts.push(VK_SAMPLE_COUNT_1_BIT);

            defaultDepthFormat = depthFormat;

            surfaceFormat = _FindSurfaceFormat();
        }
        //--------------------------------------------------------------------------

        VkSurfaceFormatKHR VulkanContext::_FindSurfaceFormat() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportant);

            if (surfaceFormats.empty())
            {
                KMP_LOG_CRITICAL("unable to get available surface format");
                throw std::runtime_error("VulkanContext: unable to get available surface format");
            }

            for (const auto& surfFormat : surfaceFormats)
            {
                //TODO: add to settings in a future, use default non-SRGB
                if (surfFormat.format == VK_FORMAT_B8G8R8A8_UINT && surfFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return surfFormat;
                }
            }

            return surfaceFormats[0];
        }
        //--------------------------------------------------------------------------
    }
}