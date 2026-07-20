#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_base.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //! Vulkan plain texture implementation. Such textures intended to be
        //! used in shaders, i.e. everything that user sees directly (albedo texture)
        //! or indirectly (normal maps, height maps, etc.)
        class KMP_API VulkanTexture : public Texture, public VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTexture)
            KMP_LOG_CLASSNAME(VulkanTexture)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanTexture(VkImageType imageType, VkFormat format, UInt32 mipLevels, VkDevice device, VkCommandBuffer commandBuffer, 
                          const VulkanBuffer& stagingBuffer, const VkExtent3D& extent, const VulkanImageCreatorDelegate& imageCreatorDelegate);
            ~VulkanTexture() = default;

        private:
            void _TransitionImageLayout(UInt32 mipLevels, VkCommandBuffer commandBuffer);
            void _CopyStagingBufferToImage(const VulkanBuffer& stagingBuffer, const VkExtent3D& extent, VkCommandBuffer commandBuffer);
            void _GenerateMipmaps(const VkExtent3D& extent, UInt32 mipLevels, VkCommandBuffer commandBuffer);
            void _GenerateMipmapLevel(VkImageMemoryBarrier& imageBarrier, UInt32 mipLevel, Int32& mipWidth, Int32& mipHeight, VkImage image, VkCommandBuffer commandBuffer);
        };
        //--------------------------------------------------------------------------
    }
}