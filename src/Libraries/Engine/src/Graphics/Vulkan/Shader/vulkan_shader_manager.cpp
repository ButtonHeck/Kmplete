#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_manager.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanShaderManager::VulkanShaderManager(VkDevice device, const VulkanDescriptorSetManager& descriptorSetManager)
            : _device(device)
            , _descriptorSetManager(descriptorSetManager)
        {}
        //--------------------------------------------------------------------------

        VulkanShaderManager::~VulkanShaderManager() KMP_PROFILING(ProfileLevelAlways)
        {
            _shaderModules.clear();
            _shaderObjects.clear();
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanShaderModule> VulkanShaderManager::AddShaderModule(StringID moduleSid, const Filepath& filepath) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderModules.contains(moduleSid))
            {
                KMP_LOG_WARN("shader module with sid '{}' has already been created", moduleSid);
                return *_shaderModules.at(moduleSid).get();
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderModules.emplace(moduleSid, CreateUPtr<VulkanShaderModule>(_device, filepath));
                return *iterator->second.get();
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader module with sid '{}' from '{}'", moduleSid, filepath);
                return std::nullopt;
            }
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanShaderModule> VulkanShaderManager::GetShaderModule(StringID moduleSid) const noexcept KMP_PROFILING(ProfileLevelImportant)
        {
            if (_shaderModules.contains(moduleSid))
            {
                return *_shaderModules.at(moduleSid).get();
            }

            KMP_LOG_ERROR("shader module with sid '{}' not found", moduleSid);
            return std::nullopt;
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(StringID shaderSid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                                  const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderObjects.contains(shaderSid))
            {
                KMP_LOG_WARN("shader object with sid '{}' has already been created", shaderSid);
                return true;
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderObjects.emplace(shaderSid, CreateUPtr<VulkanShaderObject>(_device, filepath, stage, nextStage, linked, descriptorSetsLayouts, name));
                return hasEmplaced;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader object with sid '{}' from '{}'", shaderSid, filepath);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(StringID shaderSid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                                  const Vector<StringID>& descriptorSetsLayoutsSids, const char* name) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderObjects.contains(shaderSid))
            {
                KMP_LOG_WARN("shader object with sid '{}' has already been created", shaderSid);
                return true;
            }

            try
            {
                const auto descriptorSetsLayouts = _descriptorSetManager.GetDescriptorSetLayouts(descriptorSetsLayoutsSids);
                const auto [iterator, hasEmplaced] = _shaderObjects.emplace(shaderSid, CreateUPtr<VulkanShaderObject>(_device, filepath, stage, nextStage, linked, descriptorSetsLayouts, name));
                return hasEmplaced;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader object with sid '{}' from '{}'", shaderSid, filepath);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        VkShaderEXT VulkanShaderManager::GetVkShader(StringID shaderSid) const noexcept
        {
            if (_shaderObjects.contains(shaderSid))
            {
                const auto& shaderObject = _shaderObjects.at(shaderSid);
                if (shaderObject)
                {
                    return shaderObject->GetVkShader();
                }
                else
                {
                    KMP_LOG_ERROR("shader object with sid '{}' found, but is nullptr", shaderSid);
                    return VK_NULL_HANDLE;
                }
            }

            KMP_LOG_ERROR("shader object with sid '{}' not found", shaderSid);
            return VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------
    }
}