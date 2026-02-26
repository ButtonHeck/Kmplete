#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanCommandPool::VulkanCommandPool(VkDevice device, UInt32 graphicsQueueIndex)
            : CommandPool()
            , _device(device)
            , _commandPool(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            auto createPoolInfo = VulkanUtils::InitVkCommandPoolCreateInfo();
            createPoolInfo.queueFamilyIndex = graphicsQueueIndex;
            createPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            const auto result = vkCreateCommandPool(_device, &createPoolInfo, nullptr, &_commandPool);
            VulkanUtils::CheckResult(result, "VulkanCommandPool: failed to create command pool");
        }
        //--------------------------------------------------------------------------

        VulkanCommandPool::~VulkanCommandPool()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkDestroyCommandPool(_device, _commandPool, nullptr);
        }
        //--------------------------------------------------------------------------

        VkCommandPool VulkanCommandPool::GetVkCommandPool() const noexcept
        {
            return _commandPool;
        }
        //--------------------------------------------------------------------------
    }
}