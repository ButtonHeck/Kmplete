#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        struct PhysicalDeviceInfo;


        //TODO: comments
        class VulkanDepthStencilAttachment
        {
            KMP_DISABLE_COPY_MOVE(VulkanDepthStencilAttachment)

        public:
            KMP_API VulkanDepthStencilAttachment(const PhysicalDeviceInfo& physicalDeviceInfo, VkDevice device, const VkExtent2D& extent);
            KMP_API ~VulkanDepthStencilAttachment();

        private:
            VkDevice _device;
            VkImage _image;
            VkDeviceMemory _memory;
            VkImageView _view;
        };
        //--------------------------------------------------------------------------
    }
}