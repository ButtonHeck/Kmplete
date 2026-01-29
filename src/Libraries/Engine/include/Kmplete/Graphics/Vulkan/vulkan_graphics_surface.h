#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_surface.h"


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class VulkanGraphicsSurface : public GraphicsSurface
        {
        public:
            KMP_API explicit VulkanGraphicsSurface(Window& window);
        };
        //--------------------------------------------------------------------------
    }
}