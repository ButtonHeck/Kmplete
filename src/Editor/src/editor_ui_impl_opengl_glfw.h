#pragma once

#include "editor_ui_impl.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    class EditorUIImplOpenglGLFW : public EditorUIImpl
    {
#if defined(KMP_PROFILE)
    private:
        UPtr<ProfilerTimer> _constructorProfilerTimer;
#endif

    public:
        KMP_DISABLE_COPY_MOVE(EditorUIImplOpenglGLFW)

        explicit EditorUIImplOpenglGLFW(Window& mainWindow);
        virtual ~EditorUIImplOpenglGLFW();

        void NewFrame() override;
        void Render() override;

    private:
        void Initialize();
        void Finalize();
    };
    //--------------------------------------------------------------------------
}