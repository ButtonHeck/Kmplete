#pragma once

#include "Kmplete/Core/window.h"
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Graphics/graphics_backend.h"

namespace Kmplete
{
    class EditorUIImpl
    {
    public:
        KMP_NODISCARD static EditorUIImpl* CreateImpl(Window* window, GraphicsBackend::BackendType graphicsBackendType);

    public:
        KMP_DISABLE_COPY_MOVE(EditorUIImpl)

        virtual ~EditorUIImpl() = default;

        virtual void NewFrame() = 0;
        virtual void Render() = 0;

    protected:
        explicit EditorUIImpl(Window* window);

    protected:
        Window* _window;
    };
    //--------------------------------------------------------------------------
}