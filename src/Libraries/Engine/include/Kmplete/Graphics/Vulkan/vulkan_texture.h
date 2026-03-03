#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class Image;


        //TODO: comments
        class VulkanTexture : public Texture
        {
            KMP_DISABLE_COPY_MOVE(VulkanTexture)
            KMP_LOG_CLASSNAME(VulkanTexture)

        public:
            KMP_API VulkanTexture(VkDevice device, VkQueue graphicsQueue, const Image& image, const VulkanImageCreatorDelegate& imageCreator, 
                                  const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkCommandPool commandPool, const VulkanContext& vulkanContext);
            KMP_API ~VulkanTexture();

            KMP_NODISCARD KMP_API VkImageView GetVkImageView() const noexcept;
            KMP_NODISCARD KMP_API VkSampler GetVkSampler() const noexcept;

        private:
            void _InitializeImage(const Image& image, const VulkanImageCreatorDelegate& imageCreator);
            void _TransitionImageLayout(UInt32 mipLevels, VkCommandPool commandPool, VkQueue graphicsQueue);
            void _CopyStagingBufferToImage(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Image& image, VkCommandPool commandPool, VkQueue graphicsQueue);
            KMP_NODISCARD UPtr<VulkanBuffer> _InitializeStagingBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Image& image);
            void _GenerateMipmaps(const Image& image, const VulkanContext& vulkanContext, VkCommandPool commandPool, VkQueue graphicsQueue);
            void _InitializeImageView(const Image& image, const VulkanImageCreatorDelegate& imageCreator);
            void _InitializeSampler(const Image& image, const VulkanImageCreatorDelegate& imageCreator);

            KMP_NODISCARD VkCommandBuffer _StartSingleTimeCommandBuffer(VkCommandPool commandPool) const;
            void _EndSingleTimeCommandBuffer(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue) const;

        private:
            VkDevice _logicalDevice;
            UPtr<VulkanImage> _image;
            VkImageView _imageView;
            VkSampler _sampler;
        };
        //--------------------------------------------------------------------------
    }
}