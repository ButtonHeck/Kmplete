#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <algorithm>


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanSwapchain::VulkanSwapchain(VkDevice device, const VulkanQueue& presentationQueue, const VulkanContext& vulkanContext, const VkExtent2D& swapchainExtent, 
                                         bool vSync, const VulkanImageCreatorDelegate& imageCreatorDelegate, const UInt32& currentBufferIndex,
                                         const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores, const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores)
            : Swapchain()
            , _currentBufferIndex(currentBufferIndex)
            , _presentationQueue(presentationQueue)
            , _vulkanContext(vulkanContext)
            , _imageCreatorDelegate(imageCreatorDelegate)
            , _device(device)
            , _swapchainExtent(swapchainExtent)
            , _swapchainImageFormat(vulkanContext.surfaceFormat.format)
            , _imageIndex(0)
            , _imageCount(0)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageViews()
            , _presentCompleteSemaphores()
            , _renderCompleteSemaphores()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            KMP_ASSERT(_device);

            _Initialize(swapchainExtent, vSync, presentCompleteSemaphores, renderCompleteSemaphores);
        }
        //--------------------------------------------------------------------------

        VulkanSwapchain::~VulkanSwapchain() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::StartFrame(float /*frameTimestep*/) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto result = AcquireNextImage();
            VKUtils::CheckResult(result, "VulkanSwapchain: failed to acquire next image");
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::EndFrame() KMP_PROFILING(ProfileLevelImportant)
        {
            QueuePresent();
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::Recreate(const VkExtent2D& swapchainExtent, bool vSync, const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores,
                                       const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores) KMP_PROFILING(ProfileLevelImportant)
        {
            _Finalize();
            _Initialize(swapchainExtent, vSync, presentCompleteSemaphores, renderCompleteSemaphores);
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanSwapchain::AcquireNextImage() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_device && _swapchain && _currentBufferIndex < _presentCompleteSemaphores.size());

            return vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, _presentCompleteSemaphores[_currentBufferIndex], nullptr, &_imageIndex);
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::QueuePresent() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_swapchain && _currentBufferIndex < _renderCompleteSemaphores.size());

            auto presentInfo = VKUtils::InitVkPresentInfoKHR();
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &_swapchain;
            presentInfo.pImageIndices = &_imageIndex;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];

            _presentationQueue.Present(presentInfo);
        }}
        //--------------------------------------------------------------------------

        UInt32 VulkanSwapchain::GetImageIndex() const noexcept
        {
            return _imageIndex;
        }
        //--------------------------------------------------------------------------

        UInt32 VulkanSwapchain::GetImageCount() const noexcept
        {
            return _imageCount;
        }
        //--------------------------------------------------------------------------

        VkImage VulkanSwapchain::GetCurrentImage() const
        {
            KMP_ASSERT(_imageIndex < _swapchainImages.size());

            return _swapchainImages[_imageIndex];
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetCurrentImageView() const
        {
            KMP_ASSERT(_imageIndex < _swapchainImageViews.size());

            return _swapchainImageViews[_imageIndex];
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_Initialize(const VkExtent2D& swapchainExtent, bool vSync, const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores,
                                          const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores) KMP_PROFILING(ProfileLevelAlways)
        {
            _imageCount = NumConcurrentFrames;
            if (_vulkanContext.surfaceCapabilities.maxImageCount > 0 && _imageCount > _vulkanContext.surfaceCapabilities.maxImageCount)
            {
                _imageCount = _vulkanContext.surfaceCapabilities.maxImageCount;
            }

            _presentCompleteSemaphores = presentCompleteSemaphores;
            _renderCompleteSemaphores = renderCompleteSemaphores;
            _swapchainExtent = swapchainExtent;
            _swapchainImageFormat = _vulkanContext.surfaceFormat.format;

            _CreateSwapchainObject(vSync);
            _CreateSwapchainImages();
            _CreateSwapchainImageViews();
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_Finalize() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device && _swapchain);

            for (auto imageView : _swapchainImageViews)
            {
                KMP_ASSERT(imageView);
                vkDestroyImageView(_device, imageView, nullptr);
            }

            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        }}
        //--------------------------------------------------------------------------

        VkPresentModeKHR VulkanSwapchain::_ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes, bool vSync) const
        {
            if (presentModes.empty())
            {
                KMP_LOG_CRITICAL("unable to get available present mode");
                throw RuntimeError("VulkanSwapchain: unable to get available present mode");
            }

            if (!vSync && Utils::VectorContains(presentModes, VK_PresentMode_Immediate))
            {
                return VK_PresentMode_Immediate;
            }

            return VK_PresentMode_FIFO;
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainObject(bool vSync) KMP_PROFILING(ProfileLevelImportant)
        {
            auto swapchainCreateInfo = VKUtils::InitVkSwapchainCreateInfoKHR();
            swapchainCreateInfo.surface = _vulkanContext.surface;
            swapchainCreateInfo.minImageCount = _imageCount;
            swapchainCreateInfo.imageFormat = _vulkanContext.surfaceFormat.format;
            swapchainCreateInfo.imageColorSpace = _vulkanContext.surfaceFormat.colorSpace;
            swapchainCreateInfo.imageExtent = _swapchainExtent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_ImageUsage_ColorAttachment;
            swapchainCreateInfo.preTransform = _vulkanContext.surfaceCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_CompositeAlpha_Opaque;
            swapchainCreateInfo.presentMode = _ChoosePresentMode(_vulkanContext.presentModes, vSync);
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            UInt32 indicesArray[] = { _vulkanContext.graphicsFamilyIndex, _vulkanContext.presentFamilyIndex };
            if (_vulkanContext.graphicsFamilyIndex != _vulkanContext.presentFamilyIndex)
            {
                KMP_LOG_WARN("due to different indices of graphics and presentation queues, image sharing mode set to concurrent");
                swapchainCreateInfo.imageSharingMode = VK_Sharing_Concurrent;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                swapchainCreateInfo.imageSharingMode = VK_Sharing_Exclusive;
            }

            const auto result = vkCreateSwapchainKHR(_device, &swapchainCreateInfo, nullptr, &_swapchain);
            VKUtils::CheckResult(result, "VulkanSwapchain: failed to create swapchain");
            KMP_ASSERT(_swapchain);
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImages() KMP_PROFILING(ProfileLevelImportant)
        {
            vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, nullptr);
            _swapchainImages.resize(_imageCount);
            vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, _swapchainImages.data());

            for (const auto& swapchainImage : _swapchainImages)
            {
                if (swapchainImage == VK_NULL_HANDLE)
                {
                    KMP_LOG_CRITICAL("one of the swapchain images is invalid");
                    throw RuntimeError("VulkanSwapchain: one of the swapchain images is invalid");
                }
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImageViews() KMP_PROFILING(ProfileLevelImportant)
        {
            _swapchainImageViews.resize(_swapchainImages.size());
            for (size_t i = 0; i < _swapchainImages.size(); i++)
            {
                const auto& subresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1;
                _swapchainImageViews[i] = _imageCreatorDelegate.CreateVkImageView(_swapchainImages[i], VK_ImageView_2D, _swapchainImageFormat, subresourceRange);
            }
        }}
        //--------------------------------------------------------------------------
    }
}