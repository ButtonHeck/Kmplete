#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanImage::VulkanImage(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate, const VulkanUtils::ImageParameters& creationParameters)
            : _device(device)
            , _image(VK_NULL_HANDLE)
            , _imageMemory(VK_NULL_HANDLE)
            , _memorySize(0)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateImageObject(creationParameters);
            _AllocateImageMemory(memoryTypeDelegate, creationParameters);
        }
        //--------------------------------------------------------------------------

        VulkanImage::~VulkanImage()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkDestroyImage(_device, _image, nullptr);
            vkFreeMemory(_device, _imageMemory, nullptr);
        }
        //--------------------------------------------------------------------------

        VkImage VulkanImage::GetVkImage() const noexcept
        {
            return _image;
        }
        //--------------------------------------------------------------------------

        VkDeviceSize VulkanImage::GetMemorySize() const noexcept
        {
            return _memorySize;
        }
        //--------------------------------------------------------------------------

        void VulkanImage::_CreateImageObject(const VulkanUtils::ImageParameters& creationParameters)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto imageCreationInfo = VulkanUtils::InitVkImageCreateInfo();
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

        void VulkanImage::_AllocateImageMemory(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const VulkanUtils::ImageParameters& creationParameters)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto imageMemoryContext = memoryTypeDelegate.GetImageMemoryContext(_device, _image, creationParameters.memoryProperties);

            auto result = vkAllocateMemory(_device, &imageMemoryContext.allocateInfo, nullptr, &_imageMemory);
            if (result != VK_SUCCESS)
            {
                vkDestroyImage(_device, _image, nullptr);
                VulkanUtils::CheckResult(result, "VulkanImage: failed to allocate image memory");
            }

            result = vkBindImageMemory(_device, _image, _imageMemory, 0);
            if (result != VK_SUCCESS)
            {
                vkDestroyImage(_device, _image, nullptr);
                vkFreeMemory(_device, _imageMemory, nullptr);
                VulkanUtils::CheckResult(result, "VulkanImage: failed to bind image memory");
            }

            _memorySize = imageMemoryContext.allocateInfo.allocationSize;
        }
        //--------------------------------------------------------------------------
    }
}