#pragma once

#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
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
            KMP_API VulkanSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const PhysicalDeviceInfo& info, const VkExtent2D& swapchainExtent, const VulkanImageCreatorDelegate& imageCreatorDelegate);
            KMP_API ~VulkanSwapchain();

        private:
            KMP_NODISCARD VkPresentModeKHR _ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const;

            void _CreateSwapchainObject(const VkSurfaceKHR& surface, UInt32 imageCount);
            void _CreateSwapchainImages(UInt32 imageCount);
            void _CreateSwapchainImageViews();
            void _CreateAttachmentImages();
            void _CreateAttachmentImagesViews();

        private:
            const VkDevice& _device;
            const PhysicalDeviceInfo& _physicalDeviceInfo;
            const VkExtent2D& _swapchainExtent;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;

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