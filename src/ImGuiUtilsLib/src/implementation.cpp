#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/implementation_glfw_opengl.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Profile/profiler.h"

#include <imgui.h>
#include <forkawesome-webfont.h>


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

        void ImGuiImplementation::AddDefaultFont(float dpiScale) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto& io = ImGui::GetIO();
            const auto fontSize = 18 * dpiScale;
            const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementation::AddIconsFont(float dpiScale) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto& io = ImGui::GetIO();
            const auto fontSize = 18 * dpiScale;
            ImFontConfig iconsConfig;
            iconsConfig.MergeMode = true;
            iconsConfig.GlyphMinAdvanceX = fontSize;
            iconsConfig.PixelSnapH = true;
            iconsConfig.GlyphOffset = ImVec2(0.0f, 0.0f);
            static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
            io.Fonts->AddFontFromFileTTF(Utils::Concatenate(KMP_FONTS_FOLDER, "forkawesome-webfont.ttf").c_str(), fontSize, &iconsConfig, iconsRanges);
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementation::Stylize(float dpiScale) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            ImGui::GetStyle() = ImGuiStyle();

            auto& style = ImGui::GetStyle();
            style.FrameBorderSize = 1.0f;
            style.WindowMenuButtonPosition = ImGuiDir_None;
            style.DisabledAlpha = 0.4f;

            style.ScaleAllSizes(dpiScale);
        }
        //--------------------------------------------------------------------------
    }
}