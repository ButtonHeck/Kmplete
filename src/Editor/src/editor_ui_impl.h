#pragma once

#include "Kmplete/Window/window.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_backend.h"

namespace Kmplete
{
    class EditorUIImpl
    {
    public:
        KMP_NODISCARD static EditorUIImpl* CreateImpl(Window& mainWindow, GraphicsBackendType graphicsBackendType);

    public:
        KMP_DISABLE_COPY_MOVE(EditorUIImpl)

        virtual ~EditorUIImpl() = default;

        virtual void NewFrame() = 0;
        virtual void Render() = 0;

    protected:
        explicit EditorUIImpl(Window& mainWindow);

    protected:
        Window& _mainWindow;
    };
    //--------------------------------------------------------------------------
}