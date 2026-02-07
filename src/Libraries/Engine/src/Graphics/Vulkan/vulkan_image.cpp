#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanImage::VulkanImage(const VkDevice& device, const PhysicalDeviceInfo& physicalDeviceInfo, const Parameters& creationParameters)
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

        void VulkanImage::_CreateImageObject(const Parameters& creationParameters)
        {
            VkImageCreateInfo imageCreationInfo{};
            imageCreationInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
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

            if (vkCreateImage(_device, &imageCreationInfo, nullptr, &_image) != VK_SUCCESS)
            {
                KMP_LOG_CRITICAL("failed to create image");
                throw std::runtime_error("VulkanImage: failed to create image");
            }
        }
        //--------------------------------------------------------------------------

        void VulkanImage::_AllocateImageMemory(const PhysicalDeviceInfo& physicalDeviceInfo, const Parameters& creationParameters)
        {
            VkMemoryRequirements memoryRequirements;
            vkGetImageMemoryRequirements(_device, _image, &memoryRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memoryRequirements.size;
            allocInfo.memoryTypeIndex = physicalDeviceInfo.FindMemoryType(memoryRequirements.memoryTypeBits, creationParameters.memoryProperties);

            if (vkAllocateMemory(_device, &allocInfo, nullptr, &_imageMemory) != VK_SUCCESS)
            {
                vkDestroyImage(_device, _image, nullptr);

                KMP_LOG_CRITICAL("failed to allocate image memory");
                throw std::runtime_error("VulkanImage: failed to allocate image memory");
            }

            vkBindImageMemory(_device, _image, _imageMemory, 0);
        }
        //--------------------------------------------------------------------------
    }
}