#pragma once

#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
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
            KMP_API VulkanSwapchain(VkDevice device, VkQueue graphicsQueue, VkSurfaceKHR surface, const PhysicalDeviceInfo& info, const VkExtent2D& swapchainExtent,
                                    const VulkanImageCreatorDelegate& imageCreatorDelegate, const UInt32& currentBufferIndex,
                                    const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores, const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores);
            KMP_API ~VulkanSwapchain();

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;

            KMP_API VkResult AcquireNextImage();
            KMP_API VkResult QueuePresent();

            KMP_NODISCARD KMP_API UInt32 GetImageIndex() const noexcept;
            KMP_NODISCARD KMP_API UInt32 GetImageCount() const noexcept;

        private:
            KMP_NODISCARD VkPresentModeKHR _ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const;

            void _CreateSwapchainObject(VkSurfaceKHR surface);
            void _CreateSwapchainImages();
            void _CreateSwapchainImageViews();
            void _CreateAttachmentImages();
            void _CreateAttachmentImagesViews();

        private:
            VkDevice _device;
            VkQueue _graphicsQueue;
            const PhysicalDeviceInfo& _physicalDeviceInfo;
            const VkExtent2D& _swapchainExtent;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;
            const UInt32& _currentBufferIndex;
            const Array<VkSemaphore, NumConcurrentFrames>& _presentCompleteSemaphores;
            const Array<VkSemaphore, NumConcurrentFrames>& _renderCompleteSemaphores;

            UInt32 _imageIndex;
            UInt32 _imageCount;
            VkSwapchainKHR _swapchain;
            Vector<VkImage> _swapchainImages;
            VkFormat _swapchainImageFormat;
            Vector<VkImageView> _swapchainImageViews;

            UPtr<VulkanImage> _colorImage;
            VkImageView _colorImageView;

            UPtr<VulkanImage> _depthImage;
            VkImageView _depthImageView;
        };
        //--------------------------------------------------------------------------
    }
}