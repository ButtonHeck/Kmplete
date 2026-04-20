#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanFormatDelegate::VulkanFormatDelegate(VkPhysicalDevice physicalDevice) noexcept
            : _physicalDevice(physicalDevice)
        {}
        //--------------------------------------------------------------------------

        VkFormatProperties VulkanFormatDelegate::GetFormatProperties(VkFormat format) const KMP_PROFILING(ProfileLevelMinor)
        {
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &formatProperties);
            return formatProperties;
        }}
        //--------------------------------------------------------------------------

        VkFormat VulkanFormatDelegate::FindImageFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const KMP_PROFILING(ProfileLevelImportant)
        {
            for (auto format : candidates)
            {
                auto formatProperties = GetFormatProperties(format);

                if (tiling == VK_ImageTiling_Linear && (formatProperties.linearTilingFeatures & features) == features)
                {
                    return format;
                }
                else if (tiling == VK_ImageTiling_Optimal && (formatProperties.optimalTilingFeatures & features) == features)
                {
                    return format;
                }
            }

            KMP_LOG_CRITICAL("failed to find supported format");
            throw std::runtime_error("VulkanFormatDelegate: failed to find supported format");
        }}
        //--------------------------------------------------------------------------

        bool VulkanFormatDelegate::IsMipmapCompatible(VkFormat format) const
        {
            const auto formatProperties = GetFormatProperties(format);
            return formatProperties.optimalTilingFeatures & VK_FormatFeature_SampledImageFilterLinear;
        }
        //--------------------------------------------------------------------------
    }
}