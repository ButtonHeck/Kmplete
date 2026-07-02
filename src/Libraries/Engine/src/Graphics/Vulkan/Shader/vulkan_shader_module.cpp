#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_module.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanShaderModule::VulkanShaderModule(VkDevice device, const Filepath& filepathBinary)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _shaderModule(VK_NULL_HANDLE)
        {
            KMP_ASSERT(_device);

            _Initialize(filepathBinary);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderModule::VulkanShaderModule(VkDevice device, const BinaryBuffer32& shaderBinary)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _shaderModule(VK_NULL_HANDLE)
        {
            KMP_ASSERT(_device);

            _Initialize(shaderBinary);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& other) noexcept
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(other._device)
            , _shaderModule(other._shaderModule)
        {
            other._device = VK_NULL_HANDLE;
            other._shaderModule = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _shaderModule);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderModule& VulkanShaderModule::operator=(VulkanShaderModule&& other) noexcept KMP_PROFILING(ProfileLevelAlways)
        {
            if (this == &other)
            {
                return *this;
            }

            _Finalize();

            _device = other._device;
            _shaderModule = other._shaderModule;

            other._device = VK_NULL_HANDLE;
            other._shaderModule = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _shaderModule);

            return *this;
        }}
        //--------------------------------------------------------------------------

        VulkanShaderModule::~VulkanShaderModule() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkPipelineShaderStageCreateInfo VulkanShaderModule::GetShaderStageCreateInfo(VkShaderStageFlagBits stage, const char* entryPointName /*= "main"*/) const noexcept KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_shaderModule);

            auto shaderStageCreateInfo = VKUtils::InitVkPipelineShaderStageCreateInfo(stage);
            shaderStageCreateInfo.module = _shaderModule;
            shaderStageCreateInfo.pName = entryPointName;

            return shaderStageCreateInfo;
        }}
        //--------------------------------------------------------------------------

        void VulkanShaderModule::_Initialize(const Filepath& filepathBinary)
        {
            if (!Filesystem::FilepathExists(filepathBinary))
            {
                KMP_LOG_ERROR("shader file not found '{}'", filepathBinary);
                throw RuntimeError("VulkanShaderModule: shader file not found");
            }

            const auto shaderBinary = Filesystem::ReadFileAsBinary(filepathBinary);
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary from '{}'", filepathBinary);
                throw RuntimeError("VulkanShaderModule: failed to load shader binary");
            }

            if (shaderBinary.size() % 4 != 0)
            {
                KMP_LOG_ERROR("shader binary size is not multiple of 4 '{}'", filepathBinary);
                throw RuntimeError("VulkanShaderModule: shader binary size is not multiple of 4");
            }

            auto shaderModuleCreateInfo = VKUtils::InitVkShaderModuleCreateInfo();
            shaderModuleCreateInfo.codeSize = shaderBinary.size();
            shaderModuleCreateInfo.pCode = reinterpret_cast<const UInt32*>(shaderBinary.data());

            auto result = vkCreateShaderModule(_device, &shaderModuleCreateInfo, nullptr, &_shaderModule);
            VKUtils::CheckResult(result, "VulkanShaderModule: failed to create shader module");
            KMP_ASSERT(_shaderModule);
        }
        //--------------------------------------------------------------------------

        void VulkanShaderModule::_Initialize(const BinaryBuffer32& shaderBinary)
        {
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary - buffer is empty");
                throw RuntimeError("VulkanShaderModule: failed to load shader binary");
            }

            auto shaderModuleCreateInfo = VKUtils::InitVkShaderModuleCreateInfo();
            shaderModuleCreateInfo.codeSize = shaderBinary.size() * sizeof(UInt32);
            shaderModuleCreateInfo.pCode = shaderBinary.data();

            auto result = vkCreateShaderModule(_device, &shaderModuleCreateInfo, nullptr, &_shaderModule);
            VKUtils::CheckResult(result, "VulkanShaderModule: failed to create shader module");
            KMP_ASSERT(_shaderModule);
        }
        //--------------------------------------------------------------------------

        void VulkanShaderModule::_Finalize()
        {
            if (_device && _shaderModule)
            {
                vkDestroyShaderModule(_device, _shaderModule, nullptr);
            }
        }
        //--------------------------------------------------------------------------
    }
}