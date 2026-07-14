#pragma once

#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_queue.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //! Vulkan swapchain object implementation
        class VulkanSwapchain : public Swapchain
        {
            KMP_DISABLE_COPY_MOVE(VulkanSwapchain)
            KMP_LOG_CLASSNAME(VulkanSwapchain)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanSwapchain(VkDevice device, const VulkanQueue& presentationQueue, const VulkanContext& vulkanContext, const VkExtent2D& swapchainExtent, 
                                    bool vSync, const VulkanImageCreatorDelegate& imageCreatorDelegate, const UInt32& currentBufferIndex,
                                    const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores, const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores);
            KMP_API ~VulkanSwapchain();

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;

            KMP_API void Recreate(const VkExtent2D& swapchainExtent, bool vSync, const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores,
                                  const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores);

            KMP_API VkResult AcquireNextImage();
            KMP_API void QueuePresent();

            KMP_NODISCARD KMP_API UInt32 GetImageIndex() const noexcept;
            KMP_NODISCARD KMP_API UInt32 GetImageCount() const noexcept;
            KMP_NODISCARD KMP_API VkImage GetCurrentImage() const;
            KMP_NODISCARD KMP_API VkImageView GetCurrentImageView() const;

        private:
            void _Initialize(const VkExtent2D& swapchainExtent, bool vSync, const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores,
                             const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores);
            void _Finalize();

            KMP_NODISCARD VkPresentModeKHR _ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes, bool vSync) const;
            void _CreateSwapchainObject(bool vSync);
            void _CreateSwapchainImages();
            void _CreateSwapchainImageViews();

        private:
            const UInt32& _currentBufferIndex;
            const VulkanQueue& _presentationQueue;
            const VulkanContext& _vulkanContext;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;

            VkDevice _device;
            VkExtent2D _swapchainExtent;
            VkFormat _swapchainImageFormat;
            UInt32 _imageIndex;
            UInt32 _imageCount;
            VkSwapchainKHR _swapchain;
            Vector<VkImage> _swapchainImages;
            Vector<VkImageView> _swapchainImageViews;
            Array<VkSemaphore, NumConcurrentFrames> _presentCompleteSemaphores;
            Array<VkSemaphore, NumConcurrentFrames> _renderCompleteSemaphores;
        };
        //--------------------------------------------------------------------------
    }
}