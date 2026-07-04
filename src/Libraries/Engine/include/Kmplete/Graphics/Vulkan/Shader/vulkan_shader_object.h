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
        //TODO: comments
        class VulkanShaderObject
        {
            KMP_DISABLE_COPY(VulkanShaderObject)
            KMP_LOG_CLASSNAME(VulkanShaderObject)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanShaderObject(VkDevice device, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                       const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            KMP_API VulkanShaderObject(VkDevice device, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                       const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            KMP_API VulkanShaderObject(VulkanShaderObject&& other) noexcept;
            KMP_API VulkanShaderObject& operator=(VulkanShaderObject&& other) noexcept;
            KMP_API ~VulkanShaderObject();

            KMP_NODISCARD KMP_API VkShaderEXT GetVkShader() const noexcept;

        private:
            void _Initialize(const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                             const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            void _Initialize(const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                             const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            void _Finalize();

        private:
            VkDevice _device;
            VkShaderEXT _shaderObject;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanShaderObject>::value);
        static_assert(IsMoveAssignable<VulkanShaderObject>::value);
    }
}