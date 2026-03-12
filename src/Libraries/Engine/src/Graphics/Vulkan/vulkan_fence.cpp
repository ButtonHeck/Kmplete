#include "Kmplete/Graphics/Vulkan/vulkan_fence.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanFence::VulkanFence(VkDevice device, bool signaled /*= true*/)
            : _device(device)
            , _fence(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            auto fenceCreateInfo = VulkanUtils::InitVkFenceCreateInfo(signaled);
            const auto result = vkCreateFence(_device, &fenceCreateInfo, nullptr, &_fence);
            VulkanUtils::CheckResult(result, "VulkanFence: failed to create fence");
        }
        //--------------------------------------------------------------------------

        VulkanFence::VulkanFence(VulkanFence&& other) noexcept
            : _device(other._device)
            , _fence(other._fence)
        {
            other._device = VK_NULL_HANDLE;
            other._fence = VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        VulkanFence& VulkanFence::operator=(VulkanFence&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _device = other._device;
            _fence = other._fence;

            other._device = VK_NULL_HANDLE;
            other._fence = VK_NULL_HANDLE;

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanFence::~VulkanFence()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (_device && _fence)
            {
                vkDestroyFence(_device, _fence, nullptr);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanFence::Wait(UInt64 timeout) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            const auto result = vkWaitForFences(_device, 1, &_fence, VK_TRUE, timeout);
            VulkanUtils::CheckResult(result, "VulkanFence: failed to wait for fence");
        }
        //--------------------------------------------------------------------------

        void VulkanFence::Reset() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            const auto result = vkResetFences(_device, 1, &_fence);
            VulkanUtils::CheckResult(result, "VulkanFence: failed to reset fence");
        }
        //--------------------------------------------------------------------------

        VkFence VulkanFence::GetVkFence() const noexcept
        {
            return _fence;
        }
        //--------------------------------------------------------------------------
    }
}