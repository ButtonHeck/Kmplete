#include "Kmplete/Graphics/Vulkan/vulkan_shader_object.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanShaderObject::VulkanShaderObject(VkDevice device, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                               const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/)
            : _device(device)
            , _shaderObject(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (!Filesystem::FilepathExists(filepath))
            {
                KMP_LOG_ERROR("shader file not found '{}'", filepath);
                throw RuntimeError("VulkanShaderObject: shader file not found");
            }

            const auto shaderBinary = Filesystem::ReadFileAsBinary(filepath);
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary from '{}'", filepath);
                throw RuntimeError("VulkanShaderObject: failed to load shader binary");
            }

            if (shaderBinary.size() % 4 != 0)
            {
                KMP_LOG_ERROR("shader binary size is not multiple of 4 '{}'", filepath);
                throw RuntimeError("VulkanShaderObject: shader binary size is not multiple of 4");
            }

            auto shaderCreateInfo = VKUtils::InitVkShaderCreateInfoEXT();
            shaderCreateInfo.stage = stage;
            shaderCreateInfo.nextStage = nextStage;
            shaderCreateInfo.flags = linked ? VK_ShaderCreate_LinkStage : 0;
            shaderCreateInfo.codeType = VK_ShaderCode_SPIRV;
            shaderCreateInfo.pName = name;
            shaderCreateInfo.setLayoutCount = UInt32(descriptorSetsLayouts.size());
            shaderCreateInfo.pSetLayouts = descriptorSetsLayouts.data();
            shaderCreateInfo.codeSize = shaderBinary.size();
            shaderCreateInfo.pCode = reinterpret_cast<const UInt32*>(shaderBinary.data());

            auto result = VKCommands::CreateShadersEXT(_device, 1, &shaderCreateInfo, nullptr, &_shaderObject);
            VKUtils::CheckResult(result, "VulkanShaderObject: failed to create shader object");
        }
        //--------------------------------------------------------------------------

        VulkanShaderObject::VulkanShaderObject(VulkanShaderObject&& other) noexcept
            : _device(other._device)
            , _shaderObject(other._shaderObject)
        {
            other._device = VK_NULL_HANDLE;
            other._shaderObject = VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        VulkanShaderObject& VulkanShaderObject::operator=(VulkanShaderObject&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            _device = other._device;
            _shaderObject = other._shaderObject;

            other._device = VK_NULL_HANDLE;
            other._shaderObject = VK_NULL_HANDLE;

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanShaderObject::~VulkanShaderObject() KMP_PROFILING(ProfileLevelAlways)
        {
            if (_device != VK_NULL_HANDLE && _shaderObject != VK_NULL_HANDLE)
            {
                VKCommands::DestroyShaderEXT(_device, _shaderObject, nullptr);
            }
        }}
        //--------------------------------------------------------------------------

        VkShaderEXT VulkanShaderObject::GetVkShader() const noexcept
        {
            return _shaderObject;
        }
        //--------------------------------------------------------------------------
    }
}