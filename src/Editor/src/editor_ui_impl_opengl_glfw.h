#pragma once

#include "editor_ui_impl.h"

namespace Kmplete
{
    class EditorUIImplOpenglGLFW : public EditorUIImpl
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUIImplOpenglGLFW)

        explicit EditorUIImplOpenglGLFW(const Ptr<Window> window);
        virtual ~EditorUIImplOpenglGLFW();

    private:
        void Initialize();
        void Finalize();
    };
    //--------------------------------------------------------------------------
}