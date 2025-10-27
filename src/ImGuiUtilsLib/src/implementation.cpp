#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/implementation_glfw_opengl.h"
#include "Kmplete/Profile/profiler.h"

#include <imgui.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ImGuiImplementation* ImGuiImplementation::CreateImpl(void* window, const String& graphicsBackendType, bool dockingEnabled, bool viewportsEnabled)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

#if defined (KMP_WINDOW_BACKEND_GLFW)
            if (graphicsBackendType == "OpenGL")
            {
                return new ImGuiImplementationGlfwOpenGL(reinterpret_cast<GLFWwindow*>(window), dockingEnabled, viewportsEnabled);
            }
#else
    #error "No suitable window/graphics backends are provided!"
#endif
            return nullptr;
        }
        //--------------------------------------------------------------------------

        ImGuiImplementation::ImGuiImplementation(bool dockingEnabled, bool viewportsEnabled)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            auto& io = ImGui::GetIO();

            if (dockingEnabled)
            {
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            }
            if (viewportsEnabled)
            {
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            }
        }
        //--------------------------------------------------------------------------

        ImGuiImplementation::~ImGuiImplementation()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            ImGui::DestroyContext();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementation::NewFrame() const
        {
            NewFrameImpl();
            ImGui::NewFrame();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementation::Render() const
        {
            ImGui::Render();
            RenderImpl();
        }
        //--------------------------------------------------------------------------
    }
}