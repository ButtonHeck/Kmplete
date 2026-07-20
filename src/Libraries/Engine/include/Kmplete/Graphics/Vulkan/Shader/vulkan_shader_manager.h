#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_object.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_module.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_load_parameters.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanDescriptorSetManager;


        //! Manager of Vulkan shader objects/modules
        //! @see VulkanShaderModule
        //! @see VulkanShaderObject
        //! @see ShaderLoadParameters
        class KMP_API VulkanShaderManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanShaderManager)
            KMP_LOG_CLASSNAME(VulkanShaderManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            //! Wrapper for shader module stage properties for pipeline creation
            struct ShaderStageInfoParameters
            {
                StringID shaderModuleSid;
                VkShaderStageFlagBits shaderModuleStages;
                const char* entryPointName;
            };

        public:
            VulkanShaderManager(VkDevice device, const VulkanDescriptorSetManager& descriptorSetManager);
            ~VulkanShaderManager();

            bool AddShaderModules(const Vector<ShaderLoadParameters>& shadersParameters);
            OptionalRef<VulkanShaderModule> AddShaderModuleFromBinaryFile(StringID moduleSid, const Filepath& filepathBinary);
            OptionalRef<VulkanShaderModule> AddShaderModuleFromBinaryCode(StringID moduleSid, const BinaryBuffer32& shaderBinary);

            KMP_NODISCARD OptionalRef<VulkanShaderModule> GetShaderModule(StringID moduleSid) const noexcept;
            KMP_NODISCARD Vector<VkPipelineShaderStageCreateInfo> GetShaderStageCreateInfos(const Vector<ShaderStageInfoParameters>& shaderModulesParameters) const noexcept;

            bool AddShaderObject(const ShaderLoadParameters& parameters, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                 const Vector<StringID>& descriptorSetsLayouts, const char* name = "main");
            bool AddShaderObject(const ShaderLoadParameters& parameters, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                 const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            bool AddShaderObject(StringID shaderSid, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                 const Vector<StringID>& descriptorSetsLayoutsSids, const char* name = "main");
            bool AddShaderObject(StringID shaderSid, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                 const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            bool AddShaderObject(StringID shaderSid, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                 const Vector<StringID>& descriptorSetsLayoutsSids, const char* name = "main");
            bool AddShaderObject(StringID shaderSid, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                 const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");

            KMP_NODISCARD VkShaderEXT GetVkShader(StringID shaderSid) const noexcept;

        private:
            KMP_NODISCARD bool _AddShaderModule(const ShaderLoadParameters& parameters);
            
        private:
            VkDevice _device;
            const VulkanDescriptorSetManager& _descriptorSetManager;

            StringIDHashMap<UPtr<VulkanShaderModule>> _shaderModules;
            StringIDHashMap<UPtr<VulkanShaderObject>> _shaderObjects;
        };
        //--------------------------------------------------------------------------
    }
}