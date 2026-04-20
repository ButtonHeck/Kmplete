#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VkFormat ImageChannelsToVkFormat(ImageChannels channels) noexcept
        {
            switch (channels)
            {
            case ImageChannels::RGBAlpha:
                return VK_Format_RGBA8_UNorm;
            case ImageChannels::RGB:
                KMP_LOG_WARN_FN("VulkanGraphicsBase: RGB channels will be converted to default RGBA VkFormat");
                return VK_Format_RGBA8_UNorm;
            case ImageChannels::GreyAlpha:
                return VK_Format_RG8_UNorm;
            case ImageChannels::Grey:
                return VK_Format_R8_UNorm;
            default:
                break;
            }

            return VK_Format_RGBA8_UNorm;
        }
        //--------------------------------------------------------------------------

        VkFormat ShaderDataTypeToVkFormat(ShaderDataType type) noexcept
        {
            switch (type)
            {
            case ShaderDataType::Float1:
                return VK_Format_R32_SFloat;
            case ShaderDataType::Float2:
                return VK_Format_RG32_SFloat;
            case ShaderDataType::Float3:
                return VK_Format_RGB32_SFloat;
            case ShaderDataType::Float4:
                return VK_Format_RGBA32_SFloat;
            default:
                break;
            }

            KMP_LOG_WARN_FN("VulkanGraphicsBase: shader data type is unknown or complementary VkFormat is not added yet");
            return VK_Format_RGB32_SFloat;
        }
        //--------------------------------------------------------------------------
    }
}