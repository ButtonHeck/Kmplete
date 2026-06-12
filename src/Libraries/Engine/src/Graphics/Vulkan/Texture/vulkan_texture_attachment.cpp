#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_attachment.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanTextureAttachment::VulkanTextureAttachment(StringID sid, VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate, VkFormat format, const VkExtent3D& extent, 
                                                         VkSampleCountFlagBits samples, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples)
            : VulkanTextureBase(device,
                VKPresets::GetImageCI_OptimalTiling_QueueExclusive_Layer1_NoLayout(VK_Image_2D, format, extent, 1, samples, usageFlags),
                VKPresets::GetImageViewCI_BaseMip0_BaseArray0_SingleLayer(VK_ImageView_2D, aspectMask, 1),
                imageCreatorDelegate,
                VK_Memory_DeviceLocal)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _sid(sid)
            , _parameters({ .format = format, .extent = extent, .usageFlags = usageFlags, .aspectMask = aspectMask, .samples = samples, .fixedSamples = fixedSamples })
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanTextureAttachment::VulkanTextureAttachment(StringID sid, VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate, const Parameters& parameters)
            : VulkanTextureBase(device,
                VKPresets::GetImageCI_OptimalTiling_QueueExclusive_Layer1_NoLayout(VK_Image_2D, parameters.format, parameters.extent, 1, parameters.samples, parameters.usageFlags),
                VKPresets::GetImageViewCI_BaseMip0_BaseArray0_SingleLayer(VK_ImageView_2D, parameters.aspectMask, 1),
                imageCreatorDelegate,
                VK_Memory_DeviceLocal)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _sid(sid)
            , _parameters(parameters)
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        StringID VulkanTextureAttachment::GetStringID() const noexcept
        {
            return _sid;
        }
        //--------------------------------------------------------------------------

        VulkanTextureAttachment::Parameters VulkanTextureAttachment::GetParameters() const noexcept
        {
            return _parameters;
        }
        //--------------------------------------------------------------------------
    }
}