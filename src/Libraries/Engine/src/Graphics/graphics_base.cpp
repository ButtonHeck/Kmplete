#include "Kmplete/Graphics/graphics_base.h"


namespace Kmplete
{
    namespace Graphics
    {
        String GraphicsBackendTypeToString(GraphicsBackendType type) noexcept
        {
            switch (type)
            {
            case GraphicsBackendType::Vulkan:
                return VulkanStr;
            default:
                return UnknownStr;
            }
        }
        //--------------------------------------------------------------------------

        GraphicsBackendType StringToGraphicsBackendType(const String& string) noexcept
        {
            if (string == VulkanStr)
            {
                return GraphicsBackendType::Vulkan;
            }

            return GraphicsBackendType::Unknown;
        }
        //--------------------------------------------------------------------------
    }
}