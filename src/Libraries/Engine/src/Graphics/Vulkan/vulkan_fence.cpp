#include "Kmplete/Graphics/Vulkan/vulkan_fence.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Core/assertion.h"
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

            _Initialize(signaled);
        }
        //--------------------------------------------------------------------------

        VulkanFence::VulkanFence(VulkanFence&& other) noexcept
            : _device(other._device)
            , _fence(other._fence)
        {
            other._device = VK_NULL_HANDLE;
            other._fence = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _fence);
        }
        //--------------------------------------------------------------------------

        VulkanFence& VulkanFence::operator=(VulkanFence&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _Finalize();

            _device = other._device;
            _fence = other._fence;

            other._device = VK_NULL_HANDLE;
            other._fence = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _fence);

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanFence::~VulkanFence() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        void VulkanFence::Wait() const
        {
            Wait(UINT64_MAX);
        }
        //--------------------------------------------------------------------------

        void VulkanFence::Wait(UInt64 timeout) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _fence);

            const auto result = vkWaitForFences(_device, 1, &_fence, VK_TRUE, timeout);
            VKUtils::CheckResult(result, "VulkanFence: failed to wait for fence");
        }}
        //--------------------------------------------------------------------------

        void VulkanFence::Reset() const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _fence);

            const auto result = vkResetFences(_device, 1, &_fence);
            VKUtils::CheckResult(result, "VulkanFence: failed to reset fence");
        }}
        //--------------------------------------------------------------------------

        VkFence VulkanFence::GetVkFence() const noexcept
        {
            KMP_ASSERT(_fence);

            return _fence;
        }
        //--------------------------------------------------------------------------

        void VulkanFence::_Initialize(bool signaled) KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device);

            auto fenceCreateInfo = VKUtils::InitVkFenceCreateInfo(signaled);
            const auto result = vkCreateFence(_device, &fenceCreateInfo, nullptr, &_fence);
            VKUtils::CheckResult(result, "VulkanFence: failed to create fence");

            KMP_ASSERT(_fence);
        }}
        //--------------------------------------------------------------------------

        void VulkanFence::_Finalize() KMP_PROFILING(ProfileLevelAlways)
        {
            if (_device && _fence)
            {
                vkDestroyFence(_device, _fence, nullptr);
            }
        }}
        //--------------------------------------------------------------------------
    }
}