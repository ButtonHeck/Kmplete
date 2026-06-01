#include "Kmplete/Graphics/Vulkan/vulkan_texture_base.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanTextureBase::VulkanTextureBase(VkDevice device, const VkImageCreateInfo& imageCreateInfo, VkImageViewCreateInfo imageViewCreateInfo,
                                             const VulkanImageCreatorDelegate& imageCreatorDelegate, VkMemoryPropertyFlagBits memoryPropertiesFlags)
            : _device(device)
            , _image(nullptr)
            , _imageView(VK_NULL_HANDLE)
        {
            KMP_ASSERT(_device);

            _InitializeImage(imageCreateInfo, imageCreatorDelegate, memoryPropertiesFlags);
            _InitializeImageView(imageViewCreateInfo, imageCreatorDelegate);
        }
        //--------------------------------------------------------------------------

        VulkanTextureBase::~VulkanTextureBase() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device && _imageView && _image);

            vkDestroyImageView(_device, _imageView, nullptr);

            _image.reset();
        }}
        //--------------------------------------------------------------------------

        VkImage VulkanTextureBase::GetVkImage() const noexcept
        {
            KMP_ASSERT(_image);

            return _image->GetVkImage();
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanTextureBase::GetVkImageView() const noexcept
        {
            KMP_ASSERT(_imageView);

            return _imageView;
        }
        //--------------------------------------------------------------------------

        VkFormat VulkanTextureBase::GetVkFormat() const noexcept
        {
            KMP_ASSERT(_image);

            return _image->GetVkFormat();
        }
        //--------------------------------------------------------------------------

        VkSampleCountFlagBits VulkanTextureBase::GetSamples() const noexcept
        {
            KMP_ASSERT(_image);

            return _image->GetSamples();
        }
        //--------------------------------------------------------------------------

        void VulkanTextureBase::_InitializeImage(const VkImageCreateInfo& imageCreateInfo, const VulkanImageCreatorDelegate& imageCreatorDelegate, VkMemoryPropertyFlagBits memoryPropertiesFlags) KMP_PROFILING(ProfileLevelImportant)
        {
            _image.reset(imageCreatorDelegate.CreateVulkanImagePtr(imageCreateInfo, memoryPropertiesFlags));
            KMP_ASSERT(_image);
        }}
        //--------------------------------------------------------------------------

        void VulkanTextureBase::_InitializeImageView(VkImageViewCreateInfo& imageViewCreateInfo, const VulkanImageCreatorDelegate& imageCreatorDelegate) KMP_PROFILING(ProfileLevelImportant)
        {
            imageViewCreateInfo.image = _image->GetVkImage();
            imageViewCreateInfo.format = _image->GetVkFormat();

            _imageView = imageCreatorDelegate.CreateVkImageView(imageViewCreateInfo);
            KMP_ASSERT(_imageView);
        }}
        //--------------------------------------------------------------------------
    }
}