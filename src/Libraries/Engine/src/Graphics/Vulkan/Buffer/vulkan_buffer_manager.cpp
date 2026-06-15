#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_buffer_creator_delegate.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBufferManager::VulkanBufferManager(VkDevice device, const VulkanBufferCreatorDelegate& bufferCreatorDelegate)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _bufferCreatorDelegate(bufferCreatorDelegate)
        {
            KMP_ASSERT(_device);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------
    }
}