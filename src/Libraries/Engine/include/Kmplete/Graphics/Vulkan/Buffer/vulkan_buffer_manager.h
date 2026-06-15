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
        class VulkanBufferCreatorDelegate;


        //TODO: comments
        class VulkanBufferManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanBufferManager)
            KMP_LOG_CLASSNAME(VulkanBufferManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanBufferManager(VkDevice device, const VulkanBufferCreatorDelegate& bufferCreatorDelegate);
            ~VulkanBufferManager() = default;

            KMP_NODISCARD KMP_API VulkanBuffer CreateBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanVertexBuffer CreateVertexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateVertexBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateIndexBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateIndexBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateUniformBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateUniformBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateStorageBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateStorageBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API VulkanBuffer CreateIndirectBuffer(const VulkanBufferParameters& parameters) const;
            KMP_API bool CreateIndirectBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> GetBuffer(StringID bufferSid) const noexcept;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> GetBuffer(StringID bufferSid, UInt32 index) const noexcept;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> GetVertexBuffer(StringID bufferSid) const noexcept;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> GetVertexBuffer(StringID bufferSid, UInt32 index) const noexcept;

        private:
            VkDevice _device;
            const VulkanBufferCreatorDelegate& _bufferCreatorDelegate;

            StringIDHashMap<UPtr<VulkanBuffer>> _buffers;
            StringIDHashMap<UPtr<VulkanVertexBuffer>> _vertexBuffers;
            StringIDHashMap<Array<UPtr<VulkanBuffer>, NumConcurrentFrames>> _perFrameBuffers;
            StringIDHashMap<Array<UPtr<VulkanVertexBuffer>, NumConcurrentFrames>> _perFrameVertexBuffers;
        };
        //--------------------------------------------------------------------------
    }
}