#include "Kmplete/ImGui/implementation_glfw_opengl.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ImGuiImplementationGlfwOpenGL::ImGuiImplementationGlfwOpenGL(ContextOpenGL* context)
            : ImGuiImplementation(context)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        {
            _Initialize();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        ImGuiImplementationGlfwOpenGL::~ImGuiImplementationGlfwOpenGL()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Finalize();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::CreateFontsTexture() const
        {
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::_Initialize() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto window = reinterpret_cast<GLFWwindow*>(_context->window);
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 330");
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::_Finalize() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::_NewFrameImpl() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwOpenGL::_RenderImpl() const
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
    }
}