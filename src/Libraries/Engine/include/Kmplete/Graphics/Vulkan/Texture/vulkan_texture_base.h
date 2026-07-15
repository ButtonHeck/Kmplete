#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_image.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //! Base class for Vulkan textures containing both an image object
        //! and a single image view associated with that image
        //! @see VulkanImage
        class VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureBase)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanTextureBase(VkDevice device, const VkImageCreateInfo& imageCreateInfo, VkImageViewCreateInfo imageViewCreateInfo,
                                      const VulkanImageCreatorDelegate& imageCreatorDelegate, VkMemoryPropertyFlagBits memoryPropertiesFlags);
            KMP_API virtual ~VulkanTextureBase();

            KMP_NODISCARD KMP_API VkImage GetVkImage() const noexcept;
            KMP_NODISCARD KMP_API VkImageView GetVkImageView() const noexcept;
            KMP_NODISCARD KMP_API VkFormat GetVkFormat() const noexcept;
            KMP_NODISCARD KMP_API VkSampleCountFlagBits GetSamples() const noexcept;

        private:
            void _InitializeImage(const VkImageCreateInfo& imageCreateInfo, const VulkanImageCreatorDelegate& imageCreatorDelegate, VkMemoryPropertyFlagBits memoryPropertiesFlags);
            void _InitializeImageView(VkImageViewCreateInfo& imageViewCreateInfo, const VulkanImageCreatorDelegate& imageCreatorDelegate);

        protected:
            VkDevice _device;
            UPtr<VulkanImage> _image;
            VkImageView _imageView;
        };
        //--------------------------------------------------------------------------
    }
}