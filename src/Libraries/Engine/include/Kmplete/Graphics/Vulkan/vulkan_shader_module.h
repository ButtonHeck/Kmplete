#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanShaderModule
        {
            KMP_DISABLE_COPY(VulkanShaderModule)
            KMP_LOG_CLASSNAME(VulkanShaderModule)

        public:
            KMP_API VulkanShaderModule(VkDevice device, const Filepath& filepath);
            KMP_API VulkanShaderModule(VulkanShaderModule&& other) noexcept;
            KMP_API VulkanShaderModule& operator=(VulkanShaderModule&& other) noexcept;
            KMP_API ~VulkanShaderModule();

            KMP_NODISCARD KMP_API VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* entryPointName = "main") const noexcept;

        private:
            VkDevice _device;
            VkShaderModule _shaderModule;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanShaderModule>::value);
        static_assert(IsMoveAssignable<VulkanShaderModule>::value);
    }
}