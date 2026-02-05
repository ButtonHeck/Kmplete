#pragma once

#include "Kmplete/Graphics/swapchain.h"
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
        class VulkanSwapchain : public Swapchain
        {
            KMP_DISABLE_COPY_MOVE(VulkanSwapchain)
            KMP_LOG_CLASSNAME(VulkanSwapchain)

        public:
            KMP_API VulkanSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const PhysicalDeviceImplementationInfo& info, const VkExtent2D& swapchainExtent, 
                                    const VkSurfaceFormatKHR& surfaceFormat, const VkFormat& depthFormat);
            KMP_API ~VulkanSwapchain();

        private:
            KMP_NODISCARD VkPresentModeKHR _ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const;

            void _CreateImageViews();
            KMP_NODISCARD VkImageView _CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels);

            void _CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
                              VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
            KMP_NODISCARD UInt32 _FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties);

        private:
            const VkDevice& _device;
            const VkSurfaceKHR& _surface;
            const PhysicalDeviceImplementationInfo& _physicalDeviceImplementationInfo;
            const VkExtent2D& _swapchainExtent;
            const VkFormat& _depthFormat;

            VkSwapchainKHR _swapchain;
            Vector<VkImage> _swapchainImages;
            VkFormat _swapchainImageFormat;
            Vector<VkImageView> _swapchainImageViews;

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