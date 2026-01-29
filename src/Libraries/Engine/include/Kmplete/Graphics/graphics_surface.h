#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class GraphicsSurface
        {
            KMP_DISABLE_COPY_MOVE(GraphicsSurface)
            KMP_LOG_CLASSNAME(GraphicsSurface)

        public:
            KMP_NODISCARD KMP_API static UPtr<GraphicsSurface> Create(Window& window);

        public:
            KMP_API explicit GraphicsSurface(Window& window);

        protected:
            Window& _window;
        };
        //--------------------------------------------------------------------------
    }
}