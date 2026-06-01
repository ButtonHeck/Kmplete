#include "Kmplete/Graphics/Vulkan/vulkan_texture_attachment.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanTextureAttachment::VulkanTextureAttachment(StringID sid, VkFormat format, VkDevice device, const VkExtent3D& extent, VkSampleCountFlagBits samples, 
                                                         VkImageUsageFlagBits usageFlags, VkImageAspectFlags aspectMask, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : VulkanTextureBase(device,
                VKPresets::GetImageCI_OptimalTiling_QueueExclusive_Layer1_NoLayout(VK_Image_2D, format, extent, 1, samples, usageFlags),
                VKPresets::GetImageViewCI_BaseMip0_BaseArray0_SingleLayer(VK_ImageView_2D, aspectMask, 1),
                imageCreatorDelegate,
                VK_Memory_DeviceLocal)
            , _sid(sid)
            , _parameters({ .format = format, .usageFlags = usageFlags, .aspectMask = aspectMask })
        {}
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