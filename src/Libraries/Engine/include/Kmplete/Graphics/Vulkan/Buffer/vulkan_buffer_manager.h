#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.h"
#include "Kmplete/Profile/profiler_fwd.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanBufferManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanBufferManager)
            KMP_LOG_CLASSNAME(VulkanBufferManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanBufferManager(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate);
            ~VulkanBufferManager() = default;

            KMP_NODISCARD KMP_API VulkanBuffer CreateBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanVertexBuffer CreateVertexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateVertexBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateIndexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateIndexBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateUniformBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateUniformBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateStorageBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateStorageBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateIndirectBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateIndirectBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> GetBuffer(StringID bufferSid) const noexcept;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> GetBuffer(StringID bufferSid, UInt32 index) const noexcept;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> GetVertexBuffer(StringID bufferSid) const noexcept;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> GetVertexBuffer(StringID bufferSid, UInt32 index) const noexcept;

        private:
            KMP_NODISCARD VulkanBuffer _CreateBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD VulkanVertexBuffer _CreateVertexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanVertexBuffer*> _CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD VulkanBuffer _CreateIndexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD VulkanBuffer _CreateUniformBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateUniformBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD VulkanBuffer _CreateStorageBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateStorageBufferPtr(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD VulkanBuffer _CreateIndirectBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateIndirectBufferPtr(const VulkanBufferParameters& parameters) const;

        private:
            VkDevice _device;
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            StringIDHashMap<UPtr<VulkanBuffer>> _buffers;
            StringIDHashMap<UPtr<VulkanVertexBuffer>> _vertexBuffers;
            StringIDHashMap<Array<UPtr<VulkanBuffer>, NumConcurrentFrames>> _perFrameBuffers;
            StringIDHashMap<Array<UPtr<VulkanVertexBuffer>, NumConcurrentFrames>> _perFrameVertexBuffers;
        };
        //--------------------------------------------------------------------------
    }
}