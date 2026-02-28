#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
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
            KMP_API VulkanTexture(VkDevice device, VkQueue graphicsQueue, const Image& image, const VulkanImageCreatorDelegate& imageCreator, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkCommandPool commandPool);
            KMP_API ~VulkanTexture();

            KMP_NODISCARD KMP_API VkImageView GetVkImageView() const noexcept;
            KMP_NODISCARD KMP_API VkSampler GetVkSampler() const noexcept;

        private:
            VkDevice _logicalDevice;
            UPtr<VulkanImage> _image;
            VkImageView _imageView;
            VkSampler _sampler;
            UPtr<VulkanBuffer> _buffer;
        };
        //--------------------------------------------------------------------------
    }
}