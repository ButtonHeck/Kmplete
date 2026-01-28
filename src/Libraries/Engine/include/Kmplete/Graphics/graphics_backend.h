#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/graphics_base.h"


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //! Base class/factory for graphics backend - an object that should do necessary API initialization
        //! for graphics-related functions
        class GraphicsBackend
        {
            KMP_DISABLE_COPY_MOVE(GraphicsBackend)

        public:
            KMP_NODISCARD KMP_API static UPtr<GraphicsBackend> Create(Window& window);

        public:
            KMP_API explicit GraphicsBackend(Window& window);
            virtual ~GraphicsBackend() = default;

            KMP_NODISCARD KMP_API GraphicsBackendType GetType() const noexcept;

        protected:
            Window& _window;
        };
        //--------------------------------------------------------------------------
    }
}