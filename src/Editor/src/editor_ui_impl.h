#pragma once

#include "Kmplete/Core/window.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/kmplete_api.h"

namespace Kmplete
{
    class EditorUIImpl
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUIImpl)

        static EditorUIImpl* CreateImpl(const Ptr<Window> window);

        virtual ~EditorUIImpl() = default;

    protected:
        explicit EditorUIImpl(const Ptr<Window> window);

    protected:
        const Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}