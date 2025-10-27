#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/implementation_glfw_opengl.h"
#include "Kmplete/Profile/profiler.h"

#include <imgui.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ImGuiImplementation* ImGuiImplementation::CreateImpl(void* window, const String& graphicsBackendType)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

#if defined (KMP_WINDOW_BACKEND_GLFW)
            if (graphicsBackendType == "OpenGL")
            {
                return new ImGuiImplementationGlfwOpenGL(reinterpret_cast<GLFWwindow*>(window));
            }
#else
    #error "No suitable window/graphics backends are provided!"
#endif
            return nullptr;
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