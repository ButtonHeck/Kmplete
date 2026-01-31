#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_base.h"


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class GraphicsSurface
        {
            KMP_DISABLE_COPY_MOVE(GraphicsSurface)

        public:
            KMP_API explicit GraphicsSurface(Window& window);
            virtual ~GraphicsSurface() = default;

        protected:
            Window& _window;
        };
        //--------------------------------------------------------------------------
    }
}