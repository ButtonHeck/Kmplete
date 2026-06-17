#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanPipelineCache
        {
            KMP_DISABLE_COPY_MOVE(VulkanPipelineCache)
            KMP_LOG_CLASSNAME(VulkanPipelineCache)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanPipelineCache(VkDevice device, const VulkanContext& context, const Filepath& binaryPath);
            KMP_API ~VulkanPipelineCache();

            KMP_NODISCARD KMP_API VkPipelineCache GetVkPipelineCache() const noexcept;

        private:
            struct PipelineCacheHeader
            {
                UInt32 headerLength;
                UInt32 headerVersion;
                UInt32 vendorID;
                UInt8 pipelineCacheUUID[VK_UUID_SIZE];
            };

        private:
            void _Initialize();
            void _Finalize();

        private:
            VkDevice _device;
            const VulkanContext& _context;
            VkPipelineCache _cache;
            Filepath _binaryPath;
        };
        //--------------------------------------------------------------------------
    }
}