#include "Kmplete/Graphics/Vulkan/vulkan_texture_base.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanTextureBase::VulkanTextureBase(VkDevice device)
            : _device(device)
            , _image(nullptr)
            , _imageView(VK_NULL_HANDLE)
        {
            KMP_ASSERT(_device);
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
    }
}