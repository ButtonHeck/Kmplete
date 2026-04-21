#include "Kmplete/Graphics/Vulkan/vulkan_shader_module.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanShaderModule::VulkanShaderModule(VkDevice device, const Filepath& filepath)
            : _device(device)
            , _shaderModule(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (!Filesystem::FilepathExists(filepath))
            {
                KMP_LOG_ERROR("shader file not found '{}'", filepath);
                throw RuntimeError("VulkanShaderModule: shader file not found");
            }

            const auto shaderBinary = Filesystem::ReadFileAsBinary(filepath);
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary from '{}'", filepath);
                throw RuntimeError("VulkanShaderModule: failed to load shader binary");
            }

            if (shaderBinary.size() % 4 != 0)
            {
                KMP_LOG_ERROR("shader binary size is not multiple of 4 '{}'", filepath);
                throw RuntimeError("VulkanShaderModule: shader binary size is not multiple of 4");
            }

            auto shaderModuleCreateInfo = VKUtils::InitVkShaderModuleCreateInfo();
            shaderModuleCreateInfo.codeSize = shaderBinary.size();
            shaderModuleCreateInfo.pCode = reinterpret_cast<const UInt32*>(shaderBinary.data());

            auto result = vkCreateShaderModule(_device, &shaderModuleCreateInfo, nullptr, &_shaderModule);
            VKUtils::CheckResult(result, "VulkanShaderModule: failed to create shader module");
        }
        //--------------------------------------------------------------------------

        VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& other) noexcept
            : _device(other._device)
            , _shaderModule(other._shaderModule)
        {
            other._device = VK_NULL_HANDLE;
            other._shaderModule = VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        VulkanShaderModule& VulkanShaderModule::operator=(VulkanShaderModule&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _device = other._device;
            _shaderModule = other._shaderModule;

            other._device = VK_NULL_HANDLE;
            other._shaderModule = VK_NULL_HANDLE;

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanShaderModule::~VulkanShaderModule() KMP_PROFILING(ProfileLevelAlways)
        {
            if (_device != VK_NULL_HANDLE && _shaderModule != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(_device, _shaderModule, nullptr);
            }
        }}
        //--------------------------------------------------------------------------

        VkPipelineShaderStageCreateInfo VulkanShaderModule::GetShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* entryPointName /*= "main"*/) const noexcept KMP_PROFILING(ProfileLevelMinor)
        {
            auto shaderStageCreateInfo = VKUtils::InitVkPipelineShaderStageCreateInfo(stage);
            shaderStageCreateInfo.module = _shaderModule;
            shaderStageCreateInfo.pName = entryPointName;

            return shaderStageCreateInfo;
        }}
        //--------------------------------------------------------------------------
    }
}