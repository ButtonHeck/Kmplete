#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Base/macro.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        void InitializeImGui(bool dockingEnabled, bool viewportsEnabled)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

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

        void SetItemTooltip(const char* text, ImGuiHoveredFlags_ flags /*= ImGuiHoveredFlags_AllowWhenDisabled*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            if (ImGui::IsItemHovered(flags))
            {
                KMP_COMPILER_DIAGNOSTIC_PUSH
                KMP_COMPILER_DIAGNOSTIC_IGNORE("-Wformat-security")
                ImGui::SetTooltip(text);
                KMP_COMPILER_DIAGNOSTIC_POP
            }
        }
        //--------------------------------------------------------------------------
    }
}