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


        //TODO: comments
        class VulkanTextureAttachment : public VulkanTextureBase
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureAttachment)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
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
            KMP_API VulkanTextureAttachment(StringID sid, VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate, VkFormat format, const VkExtent3D& extent, 
                                            VkSampleCountFlagBits samples, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples);
            KMP_API VulkanTextureAttachment(StringID sid, VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate, const Parameters& parameters);
            ~VulkanTextureAttachment() = default;

            KMP_NODISCARD KMP_API StringID GetStringID() const noexcept;
            KMP_NODISCARD KMP_API Parameters GetParameters() const noexcept;

        private:
            StringID _sid;
            const Parameters _parameters;
        };
        //--------------------------------------------------------------------------
    }
}