#include "Kmplete/Graphics/Vulkan/vulkan_texture_attachment_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanTextureAttachmentManager::VulkanTextureAttachmentManager(VkDevice device, const VkExtent3D& extent, VkSampleCountFlagBits msaaSamples, 
                                                                       const VulkanImageCreatorDelegate& imageCreatorDelegate, const VulkanSwapchain& swapchain)
            : _device(device)
            , _imageCreatorDelegate(imageCreatorDelegate)
            , _extent(extent)
            , _msaaSamples(msaaSamples)
            , _swapchain(swapchain)
        {}
        //--------------------------------------------------------------------------

        bool VulkanTextureAttachmentManager::AddTextureAttachment(StringID attachmentSid, VkFormat format, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples /*= false*/)
        {
            return AddTextureAttachment(attachmentSid, format, _extent, _msaaSamples, usageFlags, aspectMask, fixedSamples);
        }
        //--------------------------------------------------------------------------

        bool VulkanTextureAttachmentManager::AddTextureAttachment(StringID attachmentSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples,
                                                                  VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_textureAttachments.contains(attachmentSid))
            {
                KMP_LOG_WARN("texture attachment with sid '{}' has already been added", attachmentSid);
                return true;
            }

            const auto [iterator, hasEmplaced] = _textureAttachments.emplace(attachmentSid, CreateUPtr<VulkanTextureAttachment>(attachmentSid, _device, _imageCreatorDelegate, format, extent, samples, usageFlags, aspectMask, fixedSamples));
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanTextureAttachment> VulkanTextureAttachmentManager::GetTextureAttachment(StringID attachmentSid) const
        {
            if (_textureAttachments.contains(attachmentSid))
            {
                return std::ref(*_textureAttachments.at(attachmentSid).get());
            }

            KMP_LOG_ERROR("texture attachment with sid '{}' not found", attachmentSid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------

        VkRenderingAttachmentInfo VulkanTextureAttachmentManager::GetRenderingAttachmentInfo(VkRenderingAttachmentInfo preset, StringID imageViewAttachmentSid, StringID resolveImageViewAttachmentSid, 
                                                                                             VkResolveModeFlagBits resolveMode, VkImageLayout resolveImageLayout, bool useSwapchainForNonMSAA /*= false*/) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (!_textureAttachments.contains(imageViewAttachmentSid))
            {
                KMP_LOG_ERROR("texture attachment for image view with sid '{}' not found", imageViewAttachmentSid);
                return preset;
            }

            const auto& textureAttachment = _textureAttachments.at(imageViewAttachmentSid);
            if (textureAttachment->GetSamples() == VK_SampleCount_1)
            {
                preset.imageView = useSwapchainForNonMSAA ? _swapchain.GetCurrentImageView() : textureAttachment->GetVkImageView();
                return preset;
            }
            else
            {
                preset.imageView = textureAttachment->GetVkImageView();

                if (resolveMode == VK_Resolve_None)
                {
                    return preset;
                }

                if (resolveImageViewAttachmentSid == 0ULL)
                {
                    preset.resolveImageView = _swapchain.GetCurrentImageView();
                }
                else
                {
                    if (!_textureAttachments.contains(resolveImageViewAttachmentSid))
                    {
                        KMP_LOG_ERROR("texture attachment for resolve image view with sid '{}' not found", resolveImageViewAttachmentSid);
                        return preset;
                    }

                    preset.resolveImageView = _textureAttachments.at(resolveImageViewAttachmentSid)->GetVkImageView();
                }

                preset.resolveMode = resolveMode;
                preset.resolveImageLayout = resolveImageLayout;
                return preset;
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanTextureAttachmentManager::RecreateTextureAttachmentsWithNewSize(const VkExtent3D& newExtent) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (newExtent == _extent)
            {
                return;
            }

            _extent = newExtent;

            for (auto& [attachmentSid, textureAttachment] : _textureAttachments)
            {
                auto parameters = textureAttachment->GetParameters();
                parameters.extent = _extent;
                textureAttachment.reset(new VulkanTextureAttachment(attachmentSid, _device, _imageCreatorDelegate, parameters));
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanTextureAttachmentManager::RecreateTextureAttachmentsWithNewSamples(VkSampleCountFlagBits newSamples) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (newSamples == _msaaSamples)
            {
                return;
            }

            _msaaSamples = newSamples;

            for (auto& [attachmentSid, textureAttachment] : _textureAttachments)
            {
                auto parameters = textureAttachment->GetParameters();
                if (parameters.fixedSamples)
                {
                    continue;
                }

                parameters.samples = _msaaSamples;
                textureAttachment.reset(new VulkanTextureAttachment(attachmentSid, _device, _imageCreatorDelegate, parameters));
            }
        }}
        //--------------------------------------------------------------------------
    }
}