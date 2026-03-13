#include "Kmplete/Graphics/Vulkan/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanQueue::VulkanQueue(VkDevice device, UInt32 familyIndex, bool supportPresentation)
            : _device(device)
            , _familyIndex(familyIndex)
            , _queue(nullptr)
            , _supportPresentation(supportPresentation)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkGetDeviceQueue(_device, _familyIndex, 0, &_queue);

            if (_queue == nullptr)
            {
                KMP_LOG_CRITICAL("failed to get queue handle from logical device");
                throw std::runtime_error("VulkanQueue: failed to get queue handle from logical device");
            }
        }
        //--------------------------------------------------------------------------

        VulkanQueue::VulkanQueue(VulkanQueue&& other) noexcept
            : _device(other._device)
            , _familyIndex(other._familyIndex)
            , _queue(other._queue)
            , _supportPresentation(other._supportPresentation)
        {
            other._device = VK_NULL_HANDLE;
            other._familyIndex = UINT32_MAX;
            other._queue = nullptr;
        }
        //--------------------------------------------------------------------------

        VulkanQueue& VulkanQueue::operator=(VulkanQueue&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _device = other._device;
            _familyIndex = other._familyIndex;
            _queue = other._queue;
            _supportPresentation = other._supportPresentation;

            other._device = VK_NULL_HANDLE;
            other._familyIndex = UINT32_MAX;
            other._queue = nullptr;

            return *this;
        }
        //--------------------------------------------------------------------------

        void VulkanQueue::WaitIdle() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkQueueWaitIdle(_queue);
        }
        //--------------------------------------------------------------------------

        void VulkanQueue::Submit(const VulkanCommandBuffer& commandBuffer, VkFence fence) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto buffer = commandBuffer.GetVkCommandBuffer();
            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &buffer;

            const auto result = vkQueueSubmit(_queue, 1, &submitInfo, fence);
            VulkanUtils::CheckResult(result, "VulkanQueue: failed to submit commands to queue");
        }
        //--------------------------------------------------------------------------

        void VulkanQueue::Submit(const Vector<VkSubmitInfo>& submits, VkFence fence) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto result = vkQueueSubmit(_queue, UInt32(submits.size()), submits.data(), fence);
            VulkanUtils::CheckResult(result, "VulkanQueue: failed to submit commands to queue");
        }
        //--------------------------------------------------------------------------

        void VulkanQueue::Present(const VkPresentInfoKHR& presentationInfo) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            if (!_supportPresentation)
            {
                KMP_LOG_ERROR("current queue does not support presentation");
                return;
            }

            const auto result = vkQueuePresentKHR(_queue, &presentationInfo);
            VulkanUtils::CheckResult(result, "VulkanQueue: failed to present");
        }
        //--------------------------------------------------------------------------

        bool VulkanQueue::SupportsPresentation() const noexcept
        {
            return _supportPresentation;
        }
        //--------------------------------------------------------------------------

        VkQueue VulkanQueue::GetVkQueue() const noexcept
        {
            return _queue;
        }
        //--------------------------------------------------------------------------
    }
}