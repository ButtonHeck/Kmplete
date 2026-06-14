#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanBufferCreatorDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanBufferCreatorDelegate)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanBufferCreatorDelegate(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate);
            ~VulkanBufferCreatorDelegate() = default;

            KMP_NODISCARD KMP_API VulkanBuffer CreateBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD KMP_API VulkanVertexBuffer CreateVertexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD KMP_API VulkanBuffer CreateIndexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD KMP_API VulkanBuffer CreateUniformBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateUniformBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD KMP_API VulkanBuffer CreateStorageBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateStorageBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD KMP_API VulkanBuffer CreateIndirectBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateIndirectBufferPtr(const VulkanBufferParameters& parameters) const;

        private:
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            VkDevice _device;
        };
        //--------------------------------------------------------------------------
    }
}