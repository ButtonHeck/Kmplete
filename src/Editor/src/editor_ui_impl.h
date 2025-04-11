#pragma once

#include "Kmplete/Core/window.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/kmplete_api.h"

namespace Kmplete
{
    class EditorUIImpl
    {
    public:
        static EditorUIImpl* CreateImpl(const Ptr<Window> window, const std::string& graphicsApiName);

    public:
        KMP_DISABLE_COPY_MOVE(EditorUIImpl)

        virtual ~EditorUIImpl() = default;

        virtual void NewFrame() = 0;
        virtual void Render() = 0;

    protected:
        explicit EditorUIImpl(const Ptr<Window> window);

    protected:
        const Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}