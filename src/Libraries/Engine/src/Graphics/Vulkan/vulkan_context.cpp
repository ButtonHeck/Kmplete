#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        void VulkanContext::Populate(VkInstance vkInstance, VkPhysicalDevice physDevice, VkSurfaceKHR surfaceParam, VkFormat depthFormat, UInt32 graphicsIndex, UInt32 presentIndex,
                                     const VkSurfaceCapabilitiesKHR& surfCapabilities, Vector<VkSurfaceFormatKHR>&& surfFormats, Vector<VkPresentModeKHR>&& presentModesParam) KMP_PROFILING(ProfileLevelAlways)
        {
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
            if (samplesCountMask & VK_SampleCount_64)
                supportedSampleCounts.push(VK_SampleCount_64);
            if (samplesCountMask & VK_SampleCount_32)
                supportedSampleCounts.push(VK_SampleCount_32);
            if (samplesCountMask & VK_SampleCount_16)
                supportedSampleCounts.push(VK_SampleCount_16);
            if (samplesCountMask & VK_SampleCount_8)
                supportedSampleCounts.push(VK_SampleCount_8);
            if (samplesCountMask & VK_SampleCount_4)
                supportedSampleCounts.push(VK_SampleCount_4);
            if (samplesCountMask & VK_SampleCount_2)
                supportedSampleCounts.push(VK_SampleCount_2);
            else
                supportedSampleCounts.push(VK_SampleCount_1);

            defaultDepthFormat = depthFormat;

            surfaceFormat = _FindSurfaceFormat();
        }}
        //--------------------------------------------------------------------------

        VkSurfaceFormatKHR VulkanContext::_FindSurfaceFormat() const KMP_PROFILING(ProfileLevelImportant)
        {
            if (surfaceFormats.empty())
            {
                KMP_LOG_CRITICAL("unable to get available surface format");
                throw RuntimeError("VulkanContext: unable to get available surface format");
            }

            for (const auto& surfFormat : surfaceFormats)
            {
                //TODO: add to settings in a future, use default non-SRGB
                if (surfFormat.format == VK_Format_BGRA8_UInt && surfFormat.colorSpace == VK_ColorSpace_SRGB_Nonlinear)
                {
                    return surfFormat;
                }
            }

            return surfaceFormats[0];
        }}
        //--------------------------------------------------------------------------
    }
}