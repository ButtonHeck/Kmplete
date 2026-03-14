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
        class VulkanShader
        {
            KMP_DISABLE_COPY(VulkanShader)
            KMP_LOG_CLASSNAME(VulkanShader)

        public:
            KMP_API VulkanShader(VkDevice device, const Filepath& filepath);
            KMP_API VulkanShader(VulkanShader&& other) noexcept;
            KMP_API VulkanShader& operator=(VulkanShader&& other) noexcept;
            KMP_API ~VulkanShader();

            KMP_NODISCARD KMP_API VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* entryPointName = "main") const noexcept;

        private:
            VkDevice _device;
            VkShaderModule _shaderModule;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanShader>::value);
        static_assert(IsMoveAssignable<VulkanShader>::value);
    }
}