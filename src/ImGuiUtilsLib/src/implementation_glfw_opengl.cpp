#include "Kmplete/ImGui/implementation_glfw_opengl.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ImGuiImplementationGlfwOpenGL::ImGuiImplementationGlfwOpenGL(NonNull<GLFWwindow*> window)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS_NO_INIT_LIST("ImGuiImplementationGlfwOpenGL::ImGuiImplementationGlfwOpenGL(NonNull<GLFWwindow*>)")
        {
            Initialize(window);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        ImGuiImplementationGlfwOpenGL::~ImGuiImplementationGlfwOpenGL()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            Finalize();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::NewFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::Render()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

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

        void ImGuiImplementationGlfwOpenGL::Initialize(NonNull<GLFWwindow*> window)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 330");
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::Finalize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
        //--------------------------------------------------------------------------
    }
}