#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //! Vulkan sampler objects storage wrapper. Simplest linear/nearest filtering 
        //! samplers already registered by a logical device object during its creation.
        //! Sampler objects are referenced by StringID.
        //! @see VulkanLogicalDevice
        //! @see StringID
        class VulkanSamplersStorage
        {
            KMP_DISABLE_COPY_MOVE(VulkanSamplersStorage)
            KMP_LOG_CLASSNAME(VulkanSamplersStorage)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanSamplersStorage(VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate);
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