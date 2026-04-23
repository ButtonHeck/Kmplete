#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Core/assertion.h"
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

            _Initialize();
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

            KMP_ASSERT(_device && _commandPool && _commandBuffer);
        }
        //--------------------------------------------------------------------------

        VulkanCommandBuffer& VulkanCommandBuffer::operator=(VulkanCommandBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _Finalize();

            _device = other._device;
            _commandPool = other._commandPool;
            _commandBuffer = other._commandBuffer;

            other._device = VK_NULL_HANDLE;
            other._commandPool = VK_NULL_HANDLE;
            other._commandBuffer = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _commandPool && _commandBuffer);

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanCommandBuffer::~VulkanCommandBuffer() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags flags /*= 0*/) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_commandBuffer);

            auto commandBufferBeginInfo = VKUtils::InitVkCommandBufferBeginInfo();
            commandBufferBeginInfo.flags |= flags;

            const auto result = vkBeginCommandBuffer(_commandBuffer, &commandBufferBeginInfo);
            VKUtils::CheckResult(result, "VulkanCommandBuffer: failed to begin command buffer");
        }}
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::End() const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_commandBuffer);

            const auto result = vkEndCommandBuffer(_commandBuffer);
            VKUtils::CheckResult(result, "VulkanCommandBuffer: failed to end command buffer");
        }}
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::Reset() const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_commandBuffer);

            const auto result = vkResetCommandBuffer(_commandBuffer, 0);
            VKUtils::CheckResult(result, "VulkanCommandBuffer: failed to reset command buffer");
        }}
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanCommandBuffer::GetVkCommandBuffer() const noexcept
        {
            KMP_ASSERT(_commandBuffer);

            return _commandBuffer;
        }
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::_Initialize() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device && _commandPool);

            auto commandBufferAllocateInfo = VKUtils::InitVkCommandBufferAllocateInfo();
            commandBufferAllocateInfo.commandPool = _commandPool;
            commandBufferAllocateInfo.commandBufferCount = 1;

            const auto result = vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, &_commandBuffer);
            VKUtils::CheckResult(result, "VulkanCommandBuffer: failed to allocate command buffer");

            KMP_ASSERT(_commandBuffer);
        }}
        //--------------------------------------------------------------------------

        void VulkanCommandBuffer::_Finalize() KMP_PROFILING(ProfileLevelAlways)
        {
            if (_device && _commandPool && _commandBuffer)
            {
                vkFreeCommandBuffers(_device, _commandPool, 1, &_commandBuffer);
            }
        }}
        //--------------------------------------------------------------------------
    }
}