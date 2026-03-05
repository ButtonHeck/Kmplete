#include "Kmplete/Graphics/Vulkan/vulkan_depth_stencil_attachment.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanDepthStencilAttachment::VulkanDepthStencilAttachment(const VulkanImageCreatorDelegate& imageCreatorDelegate, VkDevice device, const VkExtent2D& extent, VkFormat depthStencilFormat)
            : _device(device)
            , _image(nullptr)
            , _view(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            auto imageCreateInfo = VulkanUtils::InitVkImageCreateInfo();
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = depthStencilFormat;
            imageCreateInfo.extent = VkExtent3D{ .width = extent.width, .height = extent.height, .depth = 1 };
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

            _image.reset(imageCreatorDelegate.CreateVulkanImagePtr(imageCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

            auto imageViewCreateInfo = VulkanUtils::InitVkImageViewCreateInfo();
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.image = _image->GetVkImage();
            imageViewCreateInfo.format = depthStencilFormat;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            if (depthStencilFormat >= VK_FORMAT_D16_UNORM_S8_UINT)
            {
                imageViewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }

            _view = imageCreatorDelegate.CreateVkImageView(imageViewCreateInfo);
        }
        //--------------------------------------------------------------------------

        VulkanDepthStencilAttachment::~VulkanDepthStencilAttachment()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkDestroyImageView(_device, _view, nullptr);
            _image.reset();
        }
        //--------------------------------------------------------------------------

        VkImage VulkanDepthStencilAttachment::GetImage() const noexcept
        {
            return _image->GetVkImage();
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanDepthStencilAttachment::GetImageView() const noexcept
        {
            return _view;
        }
        //--------------------------------------------------------------------------
    }
}