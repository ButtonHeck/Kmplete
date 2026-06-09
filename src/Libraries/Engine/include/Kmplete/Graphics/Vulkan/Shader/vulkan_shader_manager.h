#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_object.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_module.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanDescriptorSetManager;


        //TODO: comments
        class VulkanShaderManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanShaderManager)
            KMP_LOG_CLASSNAME(VulkanShaderManager)

        public:
            KMP_API VulkanShaderManager(VkDevice device, const VulkanDescriptorSetManager& descriptorSetManager);
            KMP_API ~VulkanShaderManager();

            KMP_API OptionalRef<VulkanShaderModule> AddShaderModule(StringID sid, const Filepath& filepath);
            KMP_NODISCARD KMP_API OptionalRef<VulkanShaderModule> GetShaderModule(StringID sid) const noexcept;

            KMP_API bool AddShaderObject(StringID sid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                         const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            KMP_API bool AddShaderObject(StringID sid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                         const Vector<StringID>& descriptorSetsLayoutsSids, const char* name = "main");
            KMP_NODISCARD KMP_API VkShaderEXT GetVkShader(StringID sid) const noexcept;
            
        private:
            VkDevice _device;
            const VulkanDescriptorSetManager& _descriptorSetManager;

            StringIDHashMap<UPtr<VulkanShaderModule>> _shaderModules;
            StringIDHashMap<UPtr<VulkanShaderObject>> _shaderObjects;
        };
        //--------------------------------------------------------------------------
    }
}