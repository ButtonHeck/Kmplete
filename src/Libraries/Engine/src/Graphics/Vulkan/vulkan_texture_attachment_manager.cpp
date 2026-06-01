#include "Kmplete/Graphics/Vulkan/vulkan_texture_attachment_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
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
                                                                  VkImageUsageFlagBits usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples)
        {
            if (_textureAttachments.contains(textureSid))
            {
                KMP_LOG_WARN("texture attachment with sid '{}' has already been added", textureSid);
                return true;
            }

            const auto [iterator, hasEmplaced] = _textureAttachments.emplace(textureSid, CreateUPtr<VulkanTextureAttachment>(textureSid, format, _device, extent, samples, usageFlags, aspectMask, fixedSamples, _imageCreatorDelegate));
            return hasEmplaced;
        }
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
    }
}