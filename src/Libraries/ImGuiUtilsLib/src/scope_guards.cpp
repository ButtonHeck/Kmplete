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
            KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

            if (_condition)
            {
                ImGui::BeginDisabled(_condition);
            }
        }
        //--------------------------------------------------------------------------

        DisableGuard::~DisableGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            if (_condition)
            {
                ImGui::EndDisabled();
            }
        }}
        //--------------------------------------------------------------------------


        ItemWidthGuard::ItemWidthGuard(float width) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::PushItemWidth(width);
        }}
        //--------------------------------------------------------------------------

        ItemWidthGuard::~ItemWidthGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::PopItemWidth();
        }}
        //--------------------------------------------------------------------------


        StyleColorGuard::StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors)
            : _count(static_cast<int>(colors.size()))
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

            for (const auto& value : colors)
            {
                ImGui::PushStyleColor(value.first, value.second);
            }
        }
        //--------------------------------------------------------------------------

        StyleColorGuard::~StyleColorGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::PopStyleColor(_count);
        }}
        //--------------------------------------------------------------------------


        StyleVarGuard::StyleVarGuard(std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables)
            : _count(static_cast<int>(variables.size()))
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

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

        StyleVarGuard::~StyleVarGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::PopStyleVar(_count);
        }}
        //--------------------------------------------------------------------------


        IDGuard::IDGuard(int id)
            : _id(id)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

            ImGui::PushID(_id);
        }
        //--------------------------------------------------------------------------

        IDGuard::~IDGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::PopID();
        }}
        //--------------------------------------------------------------------------


        GroupGuard::GroupGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::BeginGroup();
        }}
        //--------------------------------------------------------------------------

        GroupGuard::~GroupGuard() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            ImGui::EndGroup();
        }}
        //--------------------------------------------------------------------------
    }
}