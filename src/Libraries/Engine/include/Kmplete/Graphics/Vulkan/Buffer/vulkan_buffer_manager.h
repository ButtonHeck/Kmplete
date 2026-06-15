#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanBufferCreatorDelegate;


        //TODO: comments
        class VulkanBufferManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanBufferManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanBufferManager(VkDevice device, const VulkanBufferCreatorDelegate& bufferCreatorDelegate);
            ~VulkanBufferManager() = default;

        private:
            VkDevice _device;
            const VulkanBufferCreatorDelegate& _bufferCreatorDelegate;
        };
        //--------------------------------------------------------------------------
    }
}