#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/Profile/profiler.h"

#include <imgui_internal.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        DisableGuard::DisableGuard(bool condition)
            : _condition(condition)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            if (_condition)
            {
                ImGui::BeginDisabled();
            }
        }
        //--------------------------------------------------------------------------

        DisableGuard::~DisableGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            if (_condition)
            {
                ImGui::EndDisabled();
            }
        }
        //--------------------------------------------------------------------------


        ItemWidthGuard::ItemWidthGuard(float width)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::PushItemWidth(width);
        }
        //--------------------------------------------------------------------------

        ItemWidthGuard::~ItemWidthGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::PopItemWidth();
        }
        //--------------------------------------------------------------------------


        StyleColorGuard::StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors)
            : _count(static_cast<int>(colors.size()))
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            for (const auto& value : colors)
            {
                ImGui::PushStyleColor(value.first, value.second);
            }
        }
        //--------------------------------------------------------------------------

        StyleColorGuard::~StyleColorGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::PopStyleColor(_count);
        }
        //--------------------------------------------------------------------------


        StyleVarGuard::StyleVarGuard(std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables)
            : _count(static_cast<int>(variables.size()))
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            for (const auto& value : variables)
            {
                const auto variableInfo = ImGui::GetStyleVarInfo(value.first);

                if (variableInfo->Count == 1)
                {
                    ImGui::PushStyleVar(value.first, std::get<float>(value.second));
                }
                else if (variableInfo->Count == 2)
                {
                    ImGui::PushStyleVar(value.first, std::get<ImVec2>(value.second));
                }
            }
        }
        //--------------------------------------------------------------------------

        StyleVarGuard::~StyleVarGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::PopStyleVar(_count);
        }
        //--------------------------------------------------------------------------


        IDGuard::IDGuard(int id)
            : _id(id)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::PushID(_id);
        }
        //--------------------------------------------------------------------------

        IDGuard::~IDGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::PopID();
        }
        //--------------------------------------------------------------------------


        GroupGuard::GroupGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::BeginGroup();
        }
        //--------------------------------------------------------------------------

        GroupGuard::~GroupGuard()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            ImGui::EndGroup();
        }
        //--------------------------------------------------------------------------
    }
}