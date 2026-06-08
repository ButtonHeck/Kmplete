#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_attachment.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_swapchain.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //TODO: comments
        class VulkanTextureAttachmentManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureAttachmentManager)
            KMP_LOG_CLASSNAME(VulkanTextureAttachmentManager)

        public:
            KMP_API VulkanTextureAttachmentManager(VkDevice device, const VkExtent3D& extent, VkSampleCountFlagBits msaaSamples, 
                                                   const VulkanImageCreatorDelegate& imageCreatorDelegate, const VulkanSwapchain& swapchain);

            KMP_API bool AddTextureAttachment(StringID attachmentSid, VkFormat format, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples = false);
            KMP_API bool AddTextureAttachment(StringID attachmentSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples,
                                              VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples = false);
            KMP_NODISCARD KMP_API OptionalRef<VulkanTextureAttachment> GetTextureAttachment(StringID attachmentSid) const;

            KMP_NODISCARD KMP_API VkRenderingAttachmentInfo GetRenderingAttachmentInfo(VkRenderingAttachmentInfo preset, StringID imageViewAttachmentSid, StringID resolveImageViewAttachmentSid, 
                                                                                       VkResolveModeFlagBits resolveMode, VkImageLayout resolveImageLayout, bool useSwapchainForNonMSAA = false) const;

            KMP_API void RecreateTextureAttachmentsWithNewSize(const VkExtent3D& newExtent);
            KMP_API void RecreateTextureAttachmentsWithNewSamples(VkSampleCountFlagBits newSamples);

        private:
            VkDevice _device;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;
            VkExtent3D _extent;
            VkSampleCountFlagBits _msaaSamples;
            const VulkanSwapchain& _swapchain;

            StringIDHashMap<UPtr<VulkanTextureAttachment>> _textureAttachments;
        };
        //--------------------------------------------------------------------------
    }
}