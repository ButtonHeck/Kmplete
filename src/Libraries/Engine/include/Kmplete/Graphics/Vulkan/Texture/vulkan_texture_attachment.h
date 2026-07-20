#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_base.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //! Vulkan attachment texture implementation. Such textures intended to be used
        //! as render targets or attachments for rendering sessions
        class KMP_API VulkanTextureAttachment : public VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureAttachment)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            //! Attachment parameters wrapper, a texture attachment can be either be sample-fixed
            //! (e.g. a multisample resolve texture attachment with single sample) or not - plain 
            //! color/depth/stencil attachment for multisampled rendering
            struct Parameters
            {
                VkFormat format;
                VkExtent3D extent;
                VkImageUsageFlags usageFlags;
                VkImageAspectFlags aspectMask;
                VkSampleCountFlagBits samples;
                bool fixedSamples;
            };

        public:
            VulkanTextureAttachment(StringID sid, VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate, VkFormat format, const VkExtent3D& extent, 
                                    VkSampleCountFlagBits samples, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples);
            VulkanTextureAttachment(StringID sid, VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate, const Parameters& parameters);
            ~VulkanTextureAttachment() = default;

            KMP_NODISCARD StringID GetStringID() const noexcept;
            KMP_NODISCARD Parameters GetParameters() const noexcept;

        private:
            StringID _sid;
            const Parameters _parameters;
        };
        //--------------------------------------------------------------------------
    }
}