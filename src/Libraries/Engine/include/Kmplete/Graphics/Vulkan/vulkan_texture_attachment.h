#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture_base.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //TODO: comments
        class VulkanTextureAttachment : public VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureAttachment)
            KMP_LOG_CLASSNAME(VulkanTextureAttachment)

        public:
            KMP_API VulkanTextureAttachment(StringID sid, VkFormat format, VkDevice device, const VkExtent3D& extent, VkSampleCountFlagBits samples, 
                                            VkImageUsageFlagBits usageFlags, VkImageAspectFlags aspectMask, const VulkanImageCreatorDelegate& imageCreatorDelegate);

            KMP_NODISCARD KMP_API StringID GetStringID() const noexcept;

        private:
            StringID _sid;
        };
        //--------------------------------------------------------------------------
    }
}