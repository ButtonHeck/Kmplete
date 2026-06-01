#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture_attachment.h"
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
            KMP_API VulkanTextureAttachmentManager(VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate);

            KMP_API bool AddTextureAttachment(StringID textureSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples,
                                              VkImageUsageFlagBits usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples);
            KMP_NODISCARD KMP_API OptionalRef<VulkanTextureAttachment> GetTextureAttachment(StringID textureSid) const;

            KMP_API void RecreateTextureAttachmentsWithNewSize(const VkExtent3D& newExtent);
            KMP_API void RecreateTextureAttachmentsWithNewSamples(VkSampleCountFlagBits newSamples);

        private:
            VkDevice _device;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;

            StringIDHashMap<UPtr<VulkanTextureAttachment>> _textureAttachments;
        };
        //--------------------------------------------------------------------------
    }
}