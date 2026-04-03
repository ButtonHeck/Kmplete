#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanImage::VulkanImage(VkDevice device, const VkImageCreateInfo& creationParameters, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties)
            : _device(device)
            , _image(VK_NULL_HANDLE)
            , _imageMemory(VK_NULL_HANDLE)
            , _memorySize(0)
            , _format(creationParameters.format)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateImageObject(creationParameters);
            _AllocateImageMemory(memoryTypeDelegate, memoryProperties);
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

        VkFormat VulkanImage::GetVkFormat() const noexcept
        {
            return _format;
        }
        //--------------------------------------------------------------------------

        void VulkanImage::_CreateImageObject(const VkImageCreateInfo& creationParameters)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportant);

            const auto result = vkCreateImage(_device, &creationParameters, nullptr, &_image);
            VulkanUtils::CheckResult(result, "VulkanImage: failed to create image");
        }
        //--------------------------------------------------------------------------

        void VulkanImage::_AllocateImageMemory(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportant);

            const auto imageMemoryContext = memoryTypeDelegate.GetImageMemoryContext(_device, _image, memoryProperties);

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