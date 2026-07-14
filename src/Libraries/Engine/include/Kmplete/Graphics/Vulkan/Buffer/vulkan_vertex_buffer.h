#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //! Vulkan vertex buffer implementation class that additionally supports
        //! storing one or more BufferLayout-s which are used for calculating 
        //! bindings and attributes descriptions for rendering
        //! @see BufferLayout
        class VulkanVertexBuffer : public VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanVertexBuffer)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanVertexBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters);
            KMP_API VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept;
            KMP_API VulkanVertexBuffer& operator=(VulkanVertexBuffer&& other) noexcept;
            ~VulkanVertexBuffer() = default;

            KMP_API void AddLayout(const BufferLayout& layout);
            KMP_NODISCARD KMP_API UInt32 LayoutCount() const noexcept;

            KMP_NODISCARD KMP_API Pair<Vector<VkVertexInputBindingDescription>, Vector<VkVertexInputAttributeDescription>> GetBindingsDescriptions(UInt32 baseBinding) const noexcept;
            KMP_NODISCARD KMP_API Pair<Vector<VkVertexInputBindingDescription2EXT>, Vector<VkVertexInputAttributeDescription2EXT>> GetDynamicBindingsDescriptions(UInt32 baseBinding) const noexcept;

        private:
            //! Helper struct to store precalculated description objects for dynamic rendering.
            //! Unless a new layout is added - this cache, once calculated, may be used for dynamic rendering
            //! without recalculating each time
            struct DescriptionsCache
            {
                Vector<VkVertexInputBindingDescription2EXT> bindingDescriptions;
                Vector<VkVertexInputAttributeDescription2EXT> attributeDescriptions;
                bool valid = false;
            };

        private:
            KMP_NODISCARD Pair<Vector<VkVertexInputBindingDescription2EXT>, Vector<VkVertexInputAttributeDescription2EXT>> _GetDynamicBindingsDescriptionsFromCache(UInt32 baseBinding) const noexcept;
            KMP_NODISCARD Pair<Vector<VkVertexInputBindingDescription2EXT>, Vector<VkVertexInputAttributeDescription2EXT>> _GetDynamicBindingsDescriptions(UInt32 baseBinding) const noexcept;

        private:
            Vector<BufferLayout> _layouts;
            mutable DescriptionsCache _cache;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanVertexBuffer>::value);
        static_assert(IsMoveAssignable<VulkanVertexBuffer>::value);
    }
}