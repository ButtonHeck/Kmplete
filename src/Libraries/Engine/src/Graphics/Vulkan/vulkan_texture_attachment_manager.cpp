#include "Kmplete/Graphics/Vulkan/vulkan_texture_attachment_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanTextureAttachmentManager::VulkanTextureAttachmentManager(VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : _device(device)
            , _imageCreatorDelegate(imageCreatorDelegate)
        {}
        //--------------------------------------------------------------------------

        bool VulkanTextureAttachmentManager::AddTextureAttachment(StringID textureSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples, 
                                                                  VkImageUsageFlagBits usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_textureAttachments.contains(textureSid))
            {
                KMP_LOG_WARN("texture attachment with sid '{}' has already been added", textureSid);
                return true;
            }

            const auto [iterator, hasEmplaced] = _textureAttachments.emplace(textureSid, CreateUPtr<VulkanTextureAttachment>(textureSid, format, _device, extent, samples, usageFlags, aspectMask, fixedSamples, _imageCreatorDelegate));
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanTextureAttachment> VulkanTextureAttachmentManager::GetTextureAttachment(StringID textureSid) const
        {
            if (_textureAttachments.contains(textureSid))
            {
                return std::ref(*_textureAttachments.at(textureSid).get());
            }

            KMP_LOG_ERROR("texture attachment with sid '{}' not found", textureSid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------

        void VulkanTextureAttachmentManager::RecreateTextureAttachmentsWithNewSize(const VkExtent3D& newExtent) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            for (auto& [textureSid, textureAttachment] : _textureAttachments)
            {
                const auto parameters = textureAttachment->GetParameters();
                textureAttachment.reset(new VulkanTextureAttachment(textureSid, parameters.format, _device, newExtent, parameters.samples, parameters.usageFlags, parameters.aspectMask, parameters.fixedSamples, _imageCreatorDelegate));
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanTextureAttachmentManager::RecreateTextureAttachmentsWithNewSamples(VkSampleCountFlagBits newSamples) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            for (auto& [textureSid, textureAttachment] : _textureAttachments)
            {
                const auto parameters = textureAttachment->GetParameters();
                if (parameters.fixedSamples)
                {
                    continue;
                }

                textureAttachment.reset(new VulkanTextureAttachment(textureSid, parameters.format, _device, parameters.extent, newSamples, parameters.usageFlags, parameters.aspectMask, parameters.fixedSamples, _imageCreatorDelegate));
            }
        }}
        //--------------------------------------------------------------------------
    }
}