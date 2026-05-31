#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //TODO: comments
        class VulkanTexture : public Texture, public VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTexture)
            KMP_LOG_CLASSNAME(VulkanTexture)

        public:
            KMP_API VulkanTexture(VkFormat format, UInt32 mipLevels, VkDevice device, VkCommandBuffer commandBuffer, const VulkanBuffer& stagingBuffer, 
                                  const VkExtent3D& extent, const VulkanImageCreatorDelegate& imageCreatorDelegate);

        private:
            void _InitializeImage(VkFormat format, UInt32 mipLevels, const VkExtent3D& extent, const VulkanImageCreatorDelegate& imageCreatorDelegate);
            void _TransitionImageLayout(UInt32 mipLevels, VkCommandBuffer commandBuffer);
            void _CopyStagingBufferToImage(const VulkanBuffer& stagingBuffer, const VkExtent3D& extent, VkCommandBuffer commandBuffer);
            void _GenerateMipmaps(const VkExtent3D& extent, UInt32 mipLevels, VkCommandBuffer commandBuffer);
            void _GenerateMipmapLevel(VkImageMemoryBarrier& imageBarrier, UInt32 mipLevel, Int32& mipWidth, Int32& mipHeight, VkImage image, VkCommandBuffer commandBuffer);
            void _InitializeImageView(UInt32 mipLevels, const VulkanImageCreatorDelegate& imageCreatorDelegate);
        };
        //--------------------------------------------------------------------------
    }
}