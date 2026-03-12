#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanCommandBuffer::VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool)
            : _device(device)
            , _commandPool(commandPool)
            , _commandBuffer(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            auto commandBufferAllocateInfo = VulkanUtils::InitVkCommandBufferAllocateInfo();
            commandBufferAllocateInfo.commandPool = _commandPool;
            commandBufferAllocateInfo.commandBufferCount = 1;

            const auto result = vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, &_commandBuffer);
            VulkanUtils::CheckResult(result, "VulkanCommandBuffer: failed to allocate command buffer");
        }
        //--------------------------------------------------------------------------

        VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept
            : _device(other._device)
            , _commandPool(other._commandPool)
            , _commandBuffer(other._commandBuffer)
        {
            other._device = VK_NULL_HANDLE;
            other._commandPool = VK_NULL_HANDLE;
            other._commandBuffer = VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        VulkanCommandBuffer& VulkanCommandBuffer::operator=(VulkanCommandBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _device = other._device;
            _commandPool = other._commandPool;
            _commandBuffer = other._commandBuffer;

            other._device = VK_NULL_HANDLE;
            other._commandPool = VK_NULL_HANDLE;
            other._commandBuffer = VK_NULL_HANDLE;

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanCommandBuffer::~VulkanCommandBuffer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (_device && _commandPool && _commandBuffer)
            {
                vkFreeCommandBuffers(_device, _commandPool, 1, &_commandBuffer);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags flags /*= 0*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto commandBufferBeginInfo = VulkanUtils::InitVkCommandBufferBeginInfo();
            commandBufferBeginInfo.flags |= flags;

            const auto result = vkBeginCommandBuffer(_commandBuffer, &commandBufferBeginInfo);
            VulkanUtils::CheckResult(result, "VulkanCommandBuffer: failed to begin command buffer");
        }
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::End() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            const auto result = vkEndCommandBuffer(_commandBuffer);
            VulkanUtils::CheckResult(result, "VulkanCommandBuffer: failed to end command buffer");
        }
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::Reset() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            const auto result = vkResetCommandBuffer(_commandBuffer, 0);
            VulkanUtils::CheckResult(result, "VulkanCommandBuffer: failed to reset command buffer");
        }
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::Submit(VkQueue queue, VkFence fence /*= VK_NULL_HANDLE*/, bool waitFence /*= false*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &_commandBuffer;

            auto result = vkQueueSubmit(queue, 1, &submitInfo, fence);
            VulkanUtils::CheckResult(result, "VulkanCommandBuffer: failed to submit to queue");

            if (fence != VK_NULL_HANDLE && waitFence)
            {
                result = vkWaitForFences(_device, 1, &fence, VK_TRUE, UINT64_MAX);
                Graphics::VulkanUtils::CheckResult(result, "VulkanCommandBuffer: failed to wait on fence");
            }
        }
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanCommandBuffer::GetVkCommandBuffer() const noexcept
        {
            return _commandBuffer;
        }
        //--------------------------------------------------------------------------
    }
}