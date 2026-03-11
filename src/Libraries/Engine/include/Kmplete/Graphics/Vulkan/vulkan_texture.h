#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class Image;
        class VulkanImageCreatorDelegate;
        class VulkanFormatDelegate;


        //TODO: comments
        class VulkanTexture : public Texture
        {
            KMP_DISABLE_COPY_MOVE(VulkanTexture)
            KMP_LOG_CLASSNAME(VulkanTexture)

        public:
            KMP_API VulkanTexture(VkDevice device, VkCommandBuffer commandBuffer, const VulkanBuffer& stagingBuffer, const Image& image, 
                                  const VulkanImageCreatorDelegate& imageCreatorDelegate, const VulkanFormatDelegate& formatDelegate);
            KMP_API ~VulkanTexture();

            KMP_NODISCARD KMP_API VkImageView GetVkImageView() const noexcept;
            KMP_NODISCARD KMP_API VkSampler GetVkSampler() const noexcept;

        private:
            void _InitializeImage(const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate);
            void _TransitionImageLayout(UInt32 mipLevels, VkCommandBuffer commandBuffer);
            void _CopyStagingBufferToImage(const VulkanBuffer& stagingBuffer, const Image& image, VkCommandBuffer commandBuffer);
            void _GenerateMipmaps(const Image& image, const VulkanFormatDelegate& formatDelegate, VkCommandBuffer commandBuffer);
            void _InitializeImageView(const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate);
            void _InitializeSampler(const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate);

        private:
            VkDevice _logicalDevice;
            UPtr<VulkanImage> _image;
            VkImageView _imageView;
            VkSampler _sampler;
        };
        //--------------------------------------------------------------------------
    }
}