#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VkFormat ImageChannelsToVkFormat(ImageChannels channels)
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
    }
}