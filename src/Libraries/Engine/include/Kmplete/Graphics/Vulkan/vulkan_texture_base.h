#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureBase)

        public:
            KMP_API explicit VulkanTextureBase(VkDevice device);
            KMP_API virtual ~VulkanTextureBase();

            KMP_NODISCARD KMP_API VkImage GetVkImage() const noexcept;
            KMP_NODISCARD KMP_API VkImageView GetVkImageView() const noexcept;

        protected:
            VkDevice _device;
            UPtr<VulkanImage> _image;
            VkImageView _imageView;
        };
        //--------------------------------------------------------------------------
    }
}