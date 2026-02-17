#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
#include "Kmplete/Graphics/Vulkan/vulkan_utils.h"
#include "Kmplete/Log/log.h"

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
            auto createPoolInfo = VulkanUtils::GetVkCommandPoolCreateInfo();
            createPoolInfo.queueFamilyIndex = graphicsQueueIndex;
            createPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            const auto result = vkCreateCommandPool(_device, &createPoolInfo, nullptr, &_commandPool);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create command pool: {}", resultDescription);
                throw std::runtime_error(String("VulkanCommandPool: failed to create command pool: ").append(resultDescription));
            }
        }
        //--------------------------------------------------------------------------

        VulkanCommandPool::~VulkanCommandPool()
        {
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