#pragma once

#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
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
            KMP_API VulkanSwapchain(VkDevice device, const VulkanQueue& presentationQueue, VkSurfaceKHR surface, const VulkanContext& vulkanContext, const VkExtent2D& swapchainExtent,
                                    const VulkanImageCreatorDelegate& imageCreatorDelegate, const UInt32& currentBufferIndex,
                                    const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores, const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores);
            KMP_API ~VulkanSwapchain();

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;

            KMP_API VkResult AcquireNextImage();
            KMP_API void QueuePresent();

            KMP_NODISCARD KMP_API VkSampleCountFlagBits GetMultisampling() const noexcept;
            KMP_API void SetMultisampling(VkSampleCountFlagBits samples);

            KMP_NODISCARD KMP_API UInt32 GetImageIndex() const noexcept;
            KMP_NODISCARD KMP_API UInt32 GetImageCount() const noexcept;
            KMP_NODISCARD KMP_API VkImage GetCurrentImage() const;
            KMP_NODISCARD KMP_API VkImageView GetCurrentImageView() const;

            KMP_NODISCARD KMP_API VkImage GetMultisampledColorImage() const;
            KMP_NODISCARD KMP_API VkImage GetMultisampledDepthStencilImage() const;
            KMP_NODISCARD KMP_API VkImageView GetMultisampledColorImageView() const;
            KMP_NODISCARD KMP_API VkImageView GetMultisampledDepthStencilImageView() const;

        private:
            KMP_NODISCARD VkPresentModeKHR _ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const;

            void _CreateSwapchainObject(VkSurfaceKHR surface);
            void _CreateSwapchainImages();
            void _CreateSwapchainImageViews();
            void _CreateMultisampledAttachments();
            void _DestroyMultisamplingAttachments();

        private:
            const VulkanContext& _vulkanContext;
            const VkExtent2D& _swapchainExtent;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;
            const UInt32& _currentBufferIndex;
            const Array<VkSemaphore, NumConcurrentFrames>& _presentCompleteSemaphores;
            const Array<VkSemaphore, NumConcurrentFrames>& _renderCompleteSemaphores;
            const VulkanQueue& _presentationQueue;

            VkDevice _device;
            UInt32 _imageIndex;
            UInt32 _imageCount;
            VkSwapchainKHR _swapchain;
            Vector<VkImage> _swapchainImages;
            VkFormat _swapchainImageFormat;
            Vector<VkImageView> _swapchainImageViews;

            VkSampleCountFlagBits _msaaSamples;
            UPtr<VulkanImage> _multisampledColorImage;
            VkImageView _multisampledColorImageView;
            UPtr<VulkanImage> _multisampledDepthImage;
            VkImageView _multisampledDepthImageView;
        };
        //--------------------------------------------------------------------------
    }
}