#include "Kmplete/Graphics/Vulkan/vulkan_shader.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanShader::VulkanShader(VkDevice device, const Filepath& filepath)
            : _device(device)
            , _shaderModule(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (!Filesystem::FilepathExists(filepath))
            {
                KMP_LOG_ERROR("shader file not found '{}'", filepath);
                throw std::runtime_error("VulkanShader: shader file not found");
            }

            const auto shaderBinary = Filesystem::ReadFileAsBinary(filepath);
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary from '{}'", filepath);
                throw std::runtime_error("VulkanShader: failed to load shader binary");
            }

            if (shaderBinary.size() % 4 != 0)
            {
                KMP_LOG_ERROR("shader binary size is not multiple of 4 '{}'", filepath);
                throw std::runtime_error("VulkanShader: shader binary size is not multiple of 4");
            }

            auto shaderModuleCreateInfo = Graphics::VulkanUtils::InitVkShaderModuleCreateInfo();
            shaderModuleCreateInfo.codeSize = shaderBinary.size();
            shaderModuleCreateInfo.pCode = reinterpret_cast<const UInt32*>(shaderBinary.data());

            auto result = vkCreateShaderModule(_device, &shaderModuleCreateInfo, nullptr, &_shaderModule);
            Graphics::VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create shader module");
        }
        //--------------------------------------------------------------------------

        VulkanShader::VulkanShader(VulkanShader&& other) noexcept
            : _device(other._device)
            , _shaderModule(other._shaderModule)
        {
            other._device = VK_NULL_HANDLE;
            other._shaderModule = VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        VulkanShader& VulkanShader::operator=(VulkanShader&& other) noexcept
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

        VulkanShader::~VulkanShader()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (_device != VK_NULL_HANDLE && _shaderModule != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(_device, _shaderModule, nullptr);
            }
        }
        //--------------------------------------------------------------------------

        VkPipelineShaderStageCreateInfo VulkanShader::GetShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* entryPointName /*= "main"*/) const noexcept
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinor);

            auto shaderStageCreateInfo = VulkanUtils::InitVkPipelineShaderStageCreateInfo(stage);
            shaderStageCreateInfo.module = _shaderModule;
            shaderStageCreateInfo.pName = entryPointName;

            return shaderStageCreateInfo;
        }
        //--------------------------------------------------------------------------
    }
}