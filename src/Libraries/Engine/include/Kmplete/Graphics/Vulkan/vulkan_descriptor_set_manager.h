#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanDescriptorSetManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanDescriptorSetManager)
            KMP_LOG_CLASSNAME(VulkanDescriptorSetManager)

        public:
            KMP_API VulkanDescriptorSetManager(VkDevice device, const UInt32& currentBufferIndex);
            KMP_API ~VulkanDescriptorSetManager();

            KMP_API bool AddDescriptorSetLayout(StringID sid, const Vector<VkDescriptorSetLayoutBinding>& bindings);
            KMP_NODISCARD KMP_API VkDescriptorSetLayout GetDescriptorSetLayout(StringID sid) const noexcept;
            KMP_NODISCARD KMP_API Vector<VkDescriptorSetLayout> GetDescriptorSetLayouts(const Vector<StringID>& sids) const noexcept;

        private:
            const UInt32& _currentBufferIndex;
            VkDevice _device;
            StringIDHashMap<VkDescriptorSetLayout> _descriptorSetLayouts;
        };
        //--------------------------------------------------------------------------
    }
}