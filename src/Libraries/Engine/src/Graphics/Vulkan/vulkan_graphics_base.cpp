#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VkFormat ImageChannelsToVkFormat(ImageChannels channels) noexcept
        {
            switch (channels)
            {
            case ImageChannels::RGBAlpha:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case ImageChannels::RGB:
                KMP_LOG_WARN_FN("VulkanGraphicsBase: RGB channels will be converted to default RGBA VkFormat");
                return VK_FORMAT_R8G8B8A8_UNORM;
            case ImageChannels::GreyAlpha:
                return VK_FORMAT_R8G8_UNORM;
            case ImageChannels::Grey:
                return VK_FORMAT_R8_UNORM;
            default:
                break;
            }

            return VK_FORMAT_R8G8B8A8_UNORM;
        }
        //--------------------------------------------------------------------------

        VkFormat ShaderDataTypeToVkFormat(ShaderDataType type) noexcept
        {
            switch (type)
            {
            case ShaderDataType::Float1:
                return VK_FORMAT_R32_SFLOAT;
            case ShaderDataType::Float2:
                return VK_FORMAT_R32G32_SFLOAT;
            case ShaderDataType::Float3:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case ShaderDataType::Float4:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            default:
                break;
            }

            KMP_LOG_WARN_FN("VulkanGraphicsBase: shader data type is unknown or complementary VkFormat is not added yet");
            return VK_FORMAT_R32G32B32_SFLOAT;
        }
        //--------------------------------------------------------------------------
    }
}