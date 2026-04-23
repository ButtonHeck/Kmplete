#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanImage::VulkanImage(VkDevice device, const VkImageCreateInfo& creationParameters, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties)
            : _device(device)
            , _image(VK_NULL_HANDLE)
            , _imageMemory(VK_NULL_HANDLE)
            , _memorySize(0)
            , _format(creationParameters.format)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Initialize(creationParameters, memoryTypeDelegate, memoryProperties);
        }
        //--------------------------------------------------------------------------

        VulkanImage::VulkanImage(VulkanImage&& other) noexcept
            : _device(other._device)
            , _image(other._image)
            , _imageMemory(other._imageMemory)
            , _memorySize(other._memorySize)
            , _format(other._format)
        {
            other._device = VK_NULL_HANDLE;
            other._image = VK_NULL_HANDLE;
            other._imageMemory = VK_NULL_HANDLE;
            other._memorySize = 0ULL;
            other._format = VK_Format_Undefined;

            KMP_ASSERT(_device && _image && _imageMemory);
        }
        //--------------------------------------------------------------------------

        VulkanImage& VulkanImage::operator=(VulkanImage&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _Finalize();

            _device = other._device;
            _image = other._image;
            _imageMemory = other._imageMemory;
            _memorySize = other._memorySize;
            _format = other._format;

            other._device = VK_NULL_HANDLE;
            other._image = VK_NULL_HANDLE;
            other._imageMemory = VK_NULL_HANDLE;
            other._memorySize = 0ULL;
            other._format = VK_Format_Undefined;

            KMP_ASSERT(_device && _image && _imageMemory);

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanImage::~VulkanImage() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkImage VulkanImage::GetVkImage() const noexcept
        {
            KMP_ASSERT(_image);

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

        void VulkanImage::_Initialize(const VkImageCreateInfo& creationParameters, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties) KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device);

            auto result = vkCreateImage(_device, &creationParameters, nullptr, &_image);
            VKUtils::CheckResult(result, "VulkanImage: failed to create image");

            const auto imageMemoryContext = memoryTypeDelegate.GetImageMemoryContext(_device, _image, memoryProperties);

            result = vkAllocateMemory(_device, &imageMemoryContext.allocateInfo, nullptr, &_imageMemory);
            if (result != VK_SUCCESS)
            {
                vkDestroyImage(_device, _image, nullptr);
                VKUtils::CheckResult(result, "VulkanImage: failed to allocate image memory");
            }

            result = vkBindImageMemory(_device, _image, _imageMemory, 0);
            if (result != VK_SUCCESS)
            {
                vkDestroyImage(_device, _image, nullptr);
                vkFreeMemory(_device, _imageMemory, nullptr);
                VKUtils::CheckResult(result, "VulkanImage: failed to bind image memory");
            }

            _memorySize = imageMemoryContext.allocateInfo.allocationSize;

            KMP_ASSERT(_image && _imageMemory && _memorySize != 0);
        }}
        //--------------------------------------------------------------------------

        void VulkanImage::_Finalize() KMP_PROFILING(ProfileLevelAlways)
        {
            if (_device && _image)
            {
                vkDestroyImage(_device, _image, nullptr);
            }
            if (_device && _imageMemory)
            {
                vkFreeMemory(_device, _imageMemory, nullptr);
            }
        }}
        //--------------------------------------------------------------------------
    }
}