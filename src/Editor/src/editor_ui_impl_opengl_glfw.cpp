#include "editor_ui_impl_opengl_glfw.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Kmplete
{
    EditorUIImplOpenglGLFW::EditorUIImplOpenglGLFW(const Ptr<Window> window)
        : EditorUIImpl(window)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    EditorUIImplOpenglGLFW::~EditorUIImplOpenglGLFW()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorUIImplOpenglGLFW::Initialize()
    {
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(_window->GetImplPointer()), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
    //--------------------------------------------------------------------------

    void EditorUIImplOpenglGLFW::Finalize()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }
    //--------------------------------------------------------------------------
}