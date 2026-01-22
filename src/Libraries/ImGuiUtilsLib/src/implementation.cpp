#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/implementation_glfw_opengl.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"

#include <imgui.h>
#include <forkawesome-webfont.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ImGuiImplementation* ImGuiImplementation::CreateImpl(void* window, const String& graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, const char* configName /*= ConfigurationFileName*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

#if defined (KMP_WINDOW_BACKEND_GLFW)
            if (graphicsBackendType == "OpenGL")
            {
                return new ImGuiImplementationGlfwOpenGL(reinterpret_cast<GLFWwindow*>(window), dockingEnabled, viewportsEnabled, configName);
            }
#else
    #error "No suitable window/graphics backends are provided!"
#endif
            return nullptr;
        }
        //--------------------------------------------------------------------------

        ImGuiImplementation::ImGuiImplementation(bool dockingEnabled, bool viewportsEnabled, const char* configName /*= ConfigurationFileName*/)
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

            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
            io.IniFilename = configName;
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
            _NewFrameImpl();
            ImGui::NewFrame();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementation::Render() const
        {
            ImGui::Render();
            _RenderImpl();
        }
        //--------------------------------------------------------------------------

        bool ImGuiImplementation::AddFont(const BinaryBuffer& fontDataBuffer, float dpiScale, int fontSize /*= DefaultFontSize*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto& io = ImGui::GetIO();
            const auto fontSizeScaled = fontSize * dpiScale;
            ImFontConfig fontConfig;
            fontConfig.FontDataOwnedByAtlas = false;

            return io.Fonts->AddFontFromMemoryTTF(
                const_cast<void*>(reinterpret_cast<const void*>(fontDataBuffer.data())), 
                static_cast<int>(fontDataBuffer.size()), 
                fontSizeScaled, &fontConfig, io.Fonts->GetGlyphRangesCyrillic()
            ) != nullptr;
        }
        //--------------------------------------------------------------------------

        bool ImGuiImplementation::AddFont(const Filepath& fontFilepath, float dpiScale, int fontSize /*= DefaultFontSize*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto& io = ImGui::GetIO();
            const auto fontSizeScaled = fontSize * dpiScale;

            return io.Fonts->AddFontFromFileTTF(
                Filesystem::ToGenericString(fontFilepath).c_str(), 
                fontSizeScaled, nullptr, io.Fonts->GetGlyphRangesCyrillic()
            ) != nullptr;
        }
        //--------------------------------------------------------------------------

        bool ImGuiImplementation::AddIconsFont(const BinaryBuffer& fontDataBuffer, float dpiScale, int fontSize /*= DefaultFontSize*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto& io = ImGui::GetIO();
            const auto fontSizeScaled = fontSize * dpiScale;
            ImFontConfig fontConfig;
            fontConfig.MergeMode = true;
            fontConfig.GlyphMinAdvanceX = fontSizeScaled;
            fontConfig.PixelSnapH = true;
            fontConfig.GlyphOffset = ImVec2(0.0f, 0.0f);
            fontConfig.FontDataOwnedByAtlas = false;
            static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };

            return io.Fonts->AddFontFromMemoryTTF(
                const_cast<void*>(reinterpret_cast<const void*>(fontDataBuffer.data())), 
                static_cast<int>(fontDataBuffer.size()), 
                fontSizeScaled, &fontConfig, iconsRanges
            ) != nullptr;
        }
        //--------------------------------------------------------------------------

        bool ImGuiImplementation::AddIconsFont(const Filepath& fontFilepath, float dpiScale, int fontSize /*= DefaultFontSize*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto& io = ImGui::GetIO();
            const auto fontSizeScaled = fontSize * dpiScale;
            ImFontConfig fontConfig;
            fontConfig.MergeMode = true;
            fontConfig.GlyphMinAdvanceX = fontSizeScaled;
            fontConfig.PixelSnapH = true;
            fontConfig.GlyphOffset = ImVec2(0.0f, 0.0f);
            static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };

            return io.Fonts->AddFontFromFileTTF(
                Filesystem::ToGenericString(fontFilepath).c_str(), 
                fontSizeScaled, &fontConfig, iconsRanges
            ) != nullptr;
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