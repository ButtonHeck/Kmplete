#include "Kmplete/Graphics/Vulkan/vulkan_depth_stencil_attachment.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanDepthStencilAttachment::VulkanDepthStencilAttachment(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VkExtent2D& extent, VkFormat depthStencilFormat)
            : _device(device)
            , _image(VK_NULL_HANDLE)
            , _memory(VK_NULL_HANDLE)
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

            auto result = vkCreateImage(_device, &imageCreateInfo, nullptr, &_image);
            VulkanUtils::CheckResult(result, "VulkanDepthStencilAttachment: failed to create depth-stencil image");

            const auto imageMemoryContext = memoryTypeDelegate.GetImageMemoryContext(_device, _image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            result = vkAllocateMemory(_device, &imageMemoryContext.allocateInfo, nullptr, &_memory);
            VulkanUtils::CheckResult(result, "VulkanDepthStencilAttachment: failed to allocate depth-stencil memory");
            result = vkBindImageMemory(_device, _image, _memory, 0);
            VulkanUtils::CheckResult(result, "VulkanDepthStencilAttachment: failed to bind depth-stencil image memory");

            auto imageViewCreateInfo = VulkanUtils::InitVkImageViewCreateInfo();
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.image = _image;
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

            result = vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &_view);
            VulkanUtils::CheckResult(result, "VulkanDepthStencilAttachment: failed to create depth-stencil image view");
        }
        //--------------------------------------------------------------------------

        VulkanDepthStencilAttachment::~VulkanDepthStencilAttachment()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkDestroyImageView(_device, _view, nullptr);
            vkDestroyImage(_device, _image, nullptr);
            vkFreeMemory(_device, _memory, nullptr);
        }
        //--------------------------------------------------------------------------

        VkImage VulkanDepthStencilAttachment::GetImage() const noexcept
        {
            return _image;
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanDepthStencilAttachment::GetImageView() const noexcept
        {
            return _view;
        }
        //--------------------------------------------------------------------------
    }
}