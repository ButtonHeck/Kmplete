#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanCommandPool::VulkanCommandPool(VkDevice device, UInt32 graphicsQueueIndex)
            : CommandPool()
            , _device(device)
            , _commandPool(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            auto createPoolInfo = VKUtils::InitVkCommandPoolCreateInfo();
            createPoolInfo.queueFamilyIndex = graphicsQueueIndex;
            createPoolInfo.flags = VK_CommandPoolCreate_ResetCommandBuffer;

            const auto result = vkCreateCommandPool(_device, &createPoolInfo, nullptr, &_commandPool);
            VKUtils::CheckResult(result, "VulkanCommandPool: failed to create command pool");

            KMP_ASSERT(_device && _commandPool);
        }
        //--------------------------------------------------------------------------

        VulkanCommandPool::~VulkanCommandPool() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device && _commandPool);

            vkDestroyCommandPool(_device, _commandPool, nullptr);
        }}
        //--------------------------------------------------------------------------

        VkCommandPool VulkanCommandPool::GetVkCommandPool() const noexcept
        {
            KMP_ASSERT(_commandPool);

            return _commandPool;
        }
        //--------------------------------------------------------------------------
    }
}