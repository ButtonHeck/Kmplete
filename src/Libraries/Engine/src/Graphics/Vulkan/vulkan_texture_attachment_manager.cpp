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
                                                                  VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_textureAttachments.contains(textureSid))
            {
                KMP_LOG_WARN("texture attachment with sid '{}' has already been added", textureSid);
                return true;
            }

            const auto [iterator, hasEmplaced] = _textureAttachments.emplace(textureSid, CreateUPtr<VulkanTextureAttachment>(textureSid, _device, _imageCreatorDelegate, format, extent, samples, usageFlags, aspectMask, fixedSamples));
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
                auto parameters = textureAttachment->GetParameters();
                parameters.extent = newExtent;
                textureAttachment.reset(new VulkanTextureAttachment(textureSid, _device, _imageCreatorDelegate, parameters));
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanTextureAttachmentManager::RecreateTextureAttachmentsWithNewSamples(VkSampleCountFlagBits newSamples) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            for (auto& [textureSid, textureAttachment] : _textureAttachments)
            {
                auto parameters = textureAttachment->GetParameters();
                if (parameters.fixedSamples)
                {
                    continue;
                }

                parameters.samples = newSamples;
                textureAttachment.reset(new VulkanTextureAttachment(textureSid, _device, _imageCreatorDelegate, parameters));
            }
        }}
        //--------------------------------------------------------------------------
    }
}