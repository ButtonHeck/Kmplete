#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanFormatDelegate::VulkanFormatDelegate(VkPhysicalDevice physicalDevice) noexcept
            : _physicalDevice(physicalDevice)
        {}
        //--------------------------------------------------------------------------

        VkFormatProperties VulkanFormatDelegate::GetFormatProperties(VkFormat format) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &formatProperties);
            return formatProperties;
        }
        //--------------------------------------------------------------------------

        VkFormat VulkanFormatDelegate::FindImageFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            for (auto format : candidates)
            {
                auto formatProperties = GetFormatProperties(format);

                if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features)
                {
                    return format;
                }
                else if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features)
                {
                    return format;
                }
            }

            KMP_LOG_CRITICAL("failed to find supported format");
            throw std::runtime_error("VulkanFormatDelegate: failed to find supported format");
        }
        //--------------------------------------------------------------------------
    }
}