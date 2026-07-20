#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //! Simple Vulkan API shader module wrapper
        class KMP_API VulkanShaderModule
        {
            KMP_DISABLE_COPY(VulkanShaderModule)
            KMP_LOG_CLASSNAME(VulkanShaderModule)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanShaderModule(VkDevice device, const Filepath& filepathBinary);
            VulkanShaderModule(VkDevice device, const BinaryBuffer32& shaderBinary);
            VulkanShaderModule(VulkanShaderModule&& other) noexcept;
            VulkanShaderModule& operator=(VulkanShaderModule&& other) noexcept;
            ~VulkanShaderModule();

            KMP_NODISCARD VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* entryPointName = "main") const noexcept;

        private:
            void _Initialize(const Filepath& filepathBinary);
            void _Initialize(const BinaryBuffer32& shaderBinary);
            void _Finalize();

        private:
            VkDevice _device;
            VkShaderModule _shaderModule;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanShaderModule>::value);
        static_assert(IsMoveAssignable<VulkanShaderModule>::value);
    }
}