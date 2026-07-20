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
        class KMP_API VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureBase)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanTextureBase(VkDevice device, const VkImageCreateInfo& imageCreateInfo, VkImageViewCreateInfo imageViewCreateInfo,
                              const VulkanImageCreatorDelegate& imageCreatorDelegate, VkMemoryPropertyFlagBits memoryPropertiesFlags);
            virtual ~VulkanTextureBase();

            KMP_NODISCARD VkImage GetVkImage() const noexcept;
            KMP_NODISCARD VkImageView GetVkImageView() const noexcept;
            KMP_NODISCARD VkFormat GetVkFormat() const noexcept;
            KMP_NODISCARD VkSampleCountFlagBits GetSamples() const noexcept;

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