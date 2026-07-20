#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //! Base class of a graphics API surface abstraction
        class KMP_API GraphicsSurface
        {
            KMP_DISABLE_COPY_MOVE(GraphicsSurface)

        public:
            explicit GraphicsSurface(Window& window);
            virtual ~GraphicsSurface() = default;

        protected:
            Window& _window;
        };
        //--------------------------------------------------------------------------
    }
}