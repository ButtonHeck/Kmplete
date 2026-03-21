#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanVertexBuffer : public VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanVertexBuffer)

        public:
            KMP_API VulkanVertexBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, 
                                       VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size);
            KMP_API VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept;
            KMP_API VulkanVertexBuffer& operator=(VulkanVertexBuffer&& other) noexcept;

            KMP_API void AddLayout(const BufferLayout& layout);
            KMP_NODISCARD KMP_API UInt32 LayoutCount() const noexcept;

            KMP_NODISCARD KMP_API std::pair<Vector<VkVertexInputBindingDescription>, Vector<VkVertexInputAttributeDescription>> GetBindingsDescriptions(UInt32 baseBinding) const noexcept;

        private:
            Vector<BufferLayout> _layouts;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanVertexBuffer>::value);
        static_assert(IsMoveAssignable<VulkanVertexBuffer>::value);
    }
}