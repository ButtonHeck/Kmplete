#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_object.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanShaderObject::VulkanShaderObject(VkDevice device, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                               const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _shaderObject(VK_NULL_HANDLE)
        {
            KMP_ASSERT(_device);

            _Initialize(filepathBinary, stage, nextStage, linked, descriptorSetsLayouts, name);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderObject::VulkanShaderObject(VkDevice device, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked, 
                                               const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _shaderObject(VK_NULL_HANDLE)
        {
            KMP_ASSERT(_device);

            _Initialize(shaderBinary, stage, nextStage, linked, descriptorSetsLayouts, name);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderObject::VulkanShaderObject(VulkanShaderObject&& other) noexcept
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(other._device)
            , _shaderObject(other._shaderObject)
        {
            other._device = VK_NULL_HANDLE;
            other._shaderObject = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _shaderObject);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanShaderObject& VulkanShaderObject::operator=(VulkanShaderObject&& other) noexcept KMP_PROFILING(ProfileLevelAlways)
        {
            if (this == &other)
            {
                return *this;
            }

            _Finalize();

            _device = other._device;
            _shaderObject = other._shaderObject;

            other._device = VK_NULL_HANDLE;
            other._shaderObject = VK_NULL_HANDLE;

            KMP_ASSERT(_device && _shaderObject);

            return *this;
        }}
        //--------------------------------------------------------------------------

        VulkanShaderObject::~VulkanShaderObject() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkShaderEXT VulkanShaderObject::GetVkShader() const noexcept
        {
            return _shaderObject;
        }
        //--------------------------------------------------------------------------

        void VulkanShaderObject::_Initialize(const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                             const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/)
        {
            if (!Filesystem::FilepathExists(filepathBinary))
            {
                KMP_LOG_ERROR("shader file not found '{}'", filepathBinary);
                throw RuntimeError("VulkanShaderObject: shader file not found");
            }

            const auto shaderBinary = Filesystem::ReadFileAsBinary(filepathBinary);
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary from '{}'", filepathBinary);
                throw RuntimeError("VulkanShaderObject: failed to load shader binary");
            }

            if (shaderBinary.size() % 4 != 0)
            {
                KMP_LOG_ERROR("shader binary size is not multiple of 4 '{}'", filepathBinary);
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
            KMP_ASSERT(_shaderObject);
        }
        //--------------------------------------------------------------------------

        void VulkanShaderObject::_Initialize(const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked, 
                                             const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name /*= "main"*/)
        {
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary - buffer is empty");
                throw RuntimeError("VulkanShaderObject: failed to load shader binary");
            }

            auto shaderCreateInfo = VKUtils::InitVkShaderCreateInfoEXT();
            shaderCreateInfo.stage = stage;
            shaderCreateInfo.nextStage = nextStage;
            shaderCreateInfo.flags = linked ? VK_ShaderCreate_LinkStage : 0;
            shaderCreateInfo.codeType = VK_ShaderCode_SPIRV;
            shaderCreateInfo.pName = name;
            shaderCreateInfo.setLayoutCount = UInt32(descriptorSetsLayouts.size());
            shaderCreateInfo.pSetLayouts = descriptorSetsLayouts.data();
            shaderCreateInfo.codeSize = shaderBinary.size() * sizeof(UInt32);
            shaderCreateInfo.pCode = shaderBinary.data();

            auto result = VKCommands::CreateShadersEXT(_device, 1, &shaderCreateInfo, nullptr, &_shaderObject);
            VKUtils::CheckResult(result, "VulkanShaderObject: failed to create shader object");
            KMP_ASSERT(_shaderObject);
        }
        //--------------------------------------------------------------------------

        void VulkanShaderObject::_Finalize()
        {
            if (_device && _shaderObject)
            {
                VKCommands::DestroyShaderEXT(_device, _shaderObject, nullptr);
            }
        }
        //--------------------------------------------------------------------------
    }
}