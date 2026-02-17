#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Graphics/Vulkan/vulkan_utils.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanImage::VulkanImage(VkDevice device, const PhysicalDeviceInfo& physicalDeviceInfo, const Parameters& creationParameters)
            : _device(device)
            , _image(VK_NULL_HANDLE)
            , _imageMemory(VK_NULL_HANDLE)
        {
            _CreateImageObject(creationParameters);
            _AllocateImageMemory(physicalDeviceInfo, creationParameters);
        }
        //--------------------------------------------------------------------------

        VulkanImage::~VulkanImage()
        {
            vkDestroyImage(_device, _image, nullptr);
            vkFreeMemory(_device, _imageMemory, nullptr);
        }
        //--------------------------------------------------------------------------

        VkImage VulkanImage::GetVkImage() const noexcept
        {
            return _image;
        }
        //--------------------------------------------------------------------------

        void VulkanImage::_CreateImageObject(const Parameters& creationParameters)
        {
            auto imageCreationInfo = VulkanUtils::GetVkImageCreateInfo();
            imageCreationInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreationInfo.extent.width = creationParameters.width;
            imageCreationInfo.extent.height = creationParameters.height;
            imageCreationInfo.extent.depth = 1;
            imageCreationInfo.mipLevels = creationParameters.mipLevels;
            imageCreationInfo.arrayLayers = 1;
            imageCreationInfo.format = creationParameters.format;
            imageCreationInfo.tiling = creationParameters.tiling;
            imageCreationInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageCreationInfo.usage = creationParameters.usage;
            imageCreationInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreationInfo.samples = creationParameters.numSamples;
            imageCreationInfo.flags = 0;

            const auto result = vkCreateImage(_device, &imageCreationInfo, nullptr, &_image);
            VulkanUtils::CheckResult(result, "VulkanImage: failed to create image");
        }
        //--------------------------------------------------------------------------

        void VulkanImage::_AllocateImageMemory(const PhysicalDeviceInfo& physicalDeviceInfo, const Parameters& creationParameters)
        {
            VkMemoryRequirements memoryRequirements;
            vkGetImageMemoryRequirements(_device, _image, &memoryRequirements);

            auto memoryAllocationInfo = VulkanUtils::GetVkMemoryAllocateInfo();
            memoryAllocationInfo.allocationSize = memoryRequirements.size;
            memoryAllocationInfo.memoryTypeIndex = physicalDeviceInfo.FindMemoryType(memoryRequirements.memoryTypeBits, creationParameters.memoryProperties);

            const auto result = vkAllocateMemory(_device, &memoryAllocationInfo, nullptr, &_imageMemory);
            if (result != VK_SUCCESS)
            {
                vkDestroyImage(_device, _image, nullptr);
                VulkanUtils::CheckResult(result, "VulkanImage: failed to allocate image memory");
            }

            vkBindImageMemory(_device, _image, _imageMemory, 0);
        }
        //--------------------------------------------------------------------------
    }
}