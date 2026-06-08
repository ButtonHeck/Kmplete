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
        class VulkanImageCreatorDelegate;


        //TODO: comments
        class VulkanSamplersStorage
        {
            KMP_DISABLE_COPY_MOVE(VulkanSamplersStorage)
            KMP_LOG_CLASSNAME(VulkanSamplersStorage)

        public:
            KMP_API explicit VulkanSamplersStorage(VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate);
            KMP_API ~VulkanSamplersStorage();

            KMP_API VkSampler AddSampler(StringID sid, const VkSamplerCreateInfo& createInfo);
            KMP_NODISCARD KMP_API VkSampler GetSampler(StringID sid) const noexcept;

        private:
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;
            VkDevice _device;

            StringIDHashMap<VkSampler> _samplers;
        };
        //--------------------------------------------------------------------------
    }
}