#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //TODO: comments
        class VulkanDepthStencilAttachment
        {
            KMP_DISABLE_COPY_MOVE(VulkanDepthStencilAttachment)

        public:
            KMP_API VulkanDepthStencilAttachment(const VulkanImageCreatorDelegate& imageCreatorDelegate, VkDevice device, const VkExtent2D& extent, VkFormat depthStencilFormat);
            KMP_API ~VulkanDepthStencilAttachment();

            KMP_NODISCARD KMP_API VkImage GetImage() const noexcept;
            KMP_NODISCARD KMP_API VkImageView GetImageView() const noexcept;

        private:
            VkDevice _device;

            UPtr<VulkanImage> _image;
            VkImageView _view;
        };
        //--------------------------------------------------------------------------
    }
}