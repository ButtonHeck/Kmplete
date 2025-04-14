#include "editor_ui_impl_opengl_glfw.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Kmplete
{
    EditorUIImplOpenglGLFW::EditorUIImplOpenglGLFW(Window& mainWindow)
        : EditorUIImpl(mainWindow)
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
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(_mainWindow.GetImplPointer()), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
    //--------------------------------------------------------------------------

    void EditorUIImplOpenglGLFW::Finalize()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }
    //--------------------------------------------------------------------------

    void EditorUIImplOpenglGLFW::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUIImplOpenglGLFW::Render()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            const auto backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }
    //--------------------------------------------------------------------------
}