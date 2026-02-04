#pragma once

#include "Kmplete/Graphics/framebuffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_properties.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanFramebuffer : public Framebuffer
        {
            KMP_DISABLE_COPY_MOVE(VulkanFramebuffer)
            KMP_LOG_CLASSNAME(VulkanFramebuffer)

        public:
            KMP_API VulkanFramebuffer(const VkDevice& device, const PhysicalDeviceProperties& properties, VkFormat colorFormat, VkFormat depthFormat, const VkExtent2D& swapchainExtent, UInt32 msaaSamples);
            KMP_API ~VulkanFramebuffer();

        private:
            void _CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
                              VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

            KMP_NODISCARD VkImageView _CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels);
            KMP_NODISCARD UInt32 _FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties);

        private:
            const VkDevice& _device;
            const PhysicalDeviceProperties& _properties;

            VkFormat _colorFormat;
            VkFormat _depthFormat;

            VkImage _colorImage;
            VkDeviceMemory _colorImageMemory;
            VkImageView _colorImageView;
            
            VkImage _depthImage;
            VkDeviceMemory _depthImageMemory;
            VkImageView _depthImageView;
        };
        //--------------------------------------------------------------------------
    }
}