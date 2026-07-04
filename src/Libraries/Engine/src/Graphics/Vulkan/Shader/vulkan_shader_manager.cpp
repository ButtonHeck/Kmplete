#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_manager.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanShaderManager::VulkanShaderManager(VkDevice device, const VulkanDescriptorSetManager& descriptorSetManager)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _descriptorSetManager(descriptorSetManager)
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderManager::~VulkanShaderManager() KMP_PROFILING(ProfileLevelAlways)
        {
            _shaderModules.clear();
            _shaderObjects.clear();
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderModules(const Vector<ShaderLoadParameters>& shadersParameters) KMP_PROFILING(ProfileLevelImportant)
        {
            bool allModulesLoaded = true;
            for (const auto& parameters : shadersParameters)
            {
                allModulesLoaded &= _AddShaderModule(parameters);
            }

            return allModulesLoaded;
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanShaderModule> VulkanShaderManager::AddShaderModuleFromBinaryFile(StringID moduleSid, const Filepath& filepathBinary) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderModules.contains(moduleSid))
            {
                KMP_LOG_WARN("shader module with sid '{}' has already been created", moduleSid);
                return *_shaderModules.at(moduleSid).get();
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderModules.emplace(moduleSid, CreateUPtr<VulkanShaderModule>(_device, filepathBinary));
                return *iterator->second.get();
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader module with sid '{}' from '{}'", moduleSid, filepathBinary);
                return std::nullopt;
            }
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanShaderModule> VulkanShaderManager::AddShaderModuleFromBinaryCode(StringID moduleSid, const BinaryBuffer32& shaderBinary) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderModules.contains(moduleSid))
            {
                KMP_LOG_WARN("shader module with sid '{}' has already been created", moduleSid);
                return *_shaderModules.at(moduleSid).get();
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderModules.emplace(moduleSid, CreateUPtr<VulkanShaderModule>(_device, shaderBinary));
                return *iterator->second.get();
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader module with sid '{}' from binary", moduleSid);
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

        Vector<VkPipelineShaderStageCreateInfo> VulkanShaderManager::GetShaderStageCreateInfos(const Vector<ShaderStageInfoParameters>& shaderModulesParameters) const noexcept KMP_PROFILING(ProfileLevelImportant)
        {
            Vector<VkPipelineShaderStageCreateInfo> stagesCreateInfos;
            stagesCreateInfos.reserve(shaderModulesParameters.size());

            for (const auto& parameters : shaderModulesParameters)
            {
                const auto shaderModule = GetShaderModule(parameters.shaderModuleSid);
                if (!shaderModule.has_value())
                {
                    continue;
                }

                stagesCreateInfos.push_back(shaderModule.value().get().GetShaderStageCreateInfo(parameters.shaderModuleStages, parameters.entryPointName));
            }

            return stagesCreateInfos;
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(const ShaderLoadParameters& parameters, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                                  const Vector<StringID>& descriptorSetsLayoutsSids, const char* name /*= "main"*/) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto descriptorSetsLayouts = _descriptorSetManager.GetDescriptorSetLayouts(descriptorSetsLayoutsSids);
            return AddShaderObject(parameters, stage, nextStage, linked, descriptorSetsLayouts, name);
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(const ShaderLoadParameters& parameters, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked, 
                                                  const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (parameters.sourceType == ShaderSourceType::BinaryFile)
            {
                if (!std::holds_alternative<Filepath>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader object '{}' from binary file - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                return AddShaderObject(parameters.sid, std::get<Filepath>(parameters.source), stage, nextStage, linked, descriptorSetsLayouts, name);
            }
            else if (parameters.sourceType == ShaderSourceType::SourceFile)
            {
                if (!std::holds_alternative<Filepath>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader object '{}' from source file - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                const auto shaderSourceFile = std::get<Filepath>(parameters.source);
                const auto shaderSourceFileName = Filesystem::ToNativeString(shaderSourceFile.filename());
                const auto shaderBinary = ShaderCompiler::CompileGLSLToSpirvFromFile(shaderSourceFileName, parameters.shaderType, shaderSourceFile, "optimize"_true);
                return AddShaderObject(parameters.sid, shaderBinary, stage, nextStage, linked, descriptorSetsLayouts, name);
            }
            else if (parameters.sourceType == ShaderSourceType::BinaryCode)
            {
                if (!std::holds_alternative<BinaryBuffer32>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader object '{}' from binary code - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                return AddShaderObject(parameters.sid, std::get<BinaryBuffer32>(parameters.source), stage, nextStage, linked, descriptorSetsLayouts, name);
            }
            else if (parameters.sourceType == ShaderSourceType::SourceCode)
            {
                if (!std::holds_alternative<String>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader object '{}' from source code - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                const auto shaderBinary = ShaderCompiler::CompileGLSLToSpirvFromSource("", parameters.shaderType, std::get<String>(parameters.source), "optimize"_true);
                return AddShaderObject(parameters.sid, shaderBinary, stage, nextStage, linked, descriptorSetsLayouts, name);
            }
            else
            {
                KMP_LOG_ERROR("cannot load shader object '{}' - unknown or not implemented loading from given source type", parameters.sid);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(StringID shaderSid, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                                  const Vector<StringID>& descriptorSetsLayoutsSids, const char* name /*= "main"*/) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto descriptorSetsLayouts = _descriptorSetManager.GetDescriptorSetLayouts(descriptorSetsLayoutsSids);
            return AddShaderObject(shaderSid, filepathBinary, stage, nextStage, linked, descriptorSetsLayouts, name);
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(StringID shaderSid, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                                  const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderObjects.contains(shaderSid))
            {
                KMP_LOG_WARN("shader object with sid '{}' has already been created", shaderSid);
                return true;
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderObjects.emplace(shaderSid, CreateUPtr<VulkanShaderObject>(_device, filepathBinary, stage, nextStage, linked, descriptorSetsLayouts, name));
                return hasEmplaced;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader object with sid '{}' from '{}'", shaderSid, filepathBinary);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(StringID shaderSid, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked, 
                                                  const Vector<StringID>& descriptorSetsLayoutsSids, const char* name /*= "main"*/) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto descriptorSetsLayouts = _descriptorSetManager.GetDescriptorSetLayouts(descriptorSetsLayoutsSids);
            return AddShaderObject(shaderSid, shaderBinary, stage, nextStage, linked, descriptorSetsLayouts, name);
        }}
        //--------------------------------------------------------------------------

        bool VulkanShaderManager::AddShaderObject(StringID shaderSid, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked, 
                                                  const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderObjects.contains(shaderSid))
            {
                KMP_LOG_WARN("shader object with sid '{}' has already been created", shaderSid);
                return true;
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderObjects.emplace(shaderSid, CreateUPtr<VulkanShaderObject>(_device, shaderBinary, stage, nextStage, linked, descriptorSetsLayouts, name));
                return hasEmplaced;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader object with sid '{}' from binary buffer", shaderSid);
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

        bool VulkanShaderManager::_AddShaderModule(const ShaderLoadParameters& parameters) KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto shaderModule = OptionalRef<VulkanShaderModule>();

            if (parameters.sourceType == ShaderSourceType::BinaryFile)
            {
                if (!std::holds_alternative<Filepath>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader module '{}' from binary file - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                shaderModule = AddShaderModuleFromBinaryFile(parameters.sid, std::get<Filepath>(parameters.source));
            }
            else if (parameters.sourceType == ShaderSourceType::SourceFile)
            {
                if (!std::holds_alternative<Filepath>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader module '{}' from source file - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                const auto shaderSourceFile = std::get<Filepath>(parameters.source);
                const auto shaderSourceFileName = Filesystem::ToNativeString(shaderSourceFile.filename());
                const auto shaderBinary = ShaderCompiler::CompileGLSLToSpirvFromFile(shaderSourceFileName, parameters.shaderType, shaderSourceFile, "optimize"_true);
                shaderModule = AddShaderModuleFromBinaryCode(parameters.sid, shaderBinary);
            }
            else if (parameters.sourceType == ShaderSourceType::BinaryCode)
            {
                if (!std::holds_alternative<BinaryBuffer32>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader module '{}' from binary code - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                shaderModule = AddShaderModuleFromBinaryCode(parameters.sid, std::get<BinaryBuffer32>(parameters.source));
            }
            else if (parameters.sourceType == ShaderSourceType::SourceCode)
            {
                if (!std::holds_alternative<String>(parameters.source))
                {
                    KMP_LOG_ERROR("cannot load shader module '{}' from source code - source and type parameters mismatch", parameters.sid);
                    return false;
                }

                const auto shaderBinary = ShaderCompiler::CompileGLSLToSpirvFromSource("", parameters.shaderType, std::get<String>(parameters.source), "optimize"_true);
                shaderModule = AddShaderModuleFromBinaryCode(parameters.sid, shaderBinary);
            }
            else
            {
                KMP_LOG_ERROR("cannot load shader module '{}' - unknown or not implemented loading from given source type", parameters.sid);
                return false;
            }

            return shaderModule.has_value();
        }}
        //--------------------------------------------------------------------------
    }
}