#include "ui_utils.h"

#include <imgui_internal.h>

namespace Kmplete
{
    namespace UiUtils
    {
        DisableGuard::DisableGuard(bool condition)
            : _condition(condition)
        {
            if (_condition)
            {
                ImGui::BeginDisabled();
            }
        }
        //--------------------------------------------------------------------------

        DisableGuard::~DisableGuard()
        {
            if (_condition)
            {
                ImGui::EndDisabled();
            }
        }
        //--------------------------------------------------------------------------


        ItemWidthGuard::ItemWidthGuard(float width)
        {
            ImGui::PushItemWidth(width);
        }
        //--------------------------------------------------------------------------

        ItemWidthGuard::~ItemWidthGuard()
        {
            ImGui::PopItemWidth();
        }
        //--------------------------------------------------------------------------


        StyleColorGuard::StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors)
            : _count(static_cast<int>(colors.size()))
        {
            for (const auto& value : colors)
            {
                ImGui::PushStyleColor(value.first, value.second);
            }
        }
        //--------------------------------------------------------------------------

        StyleColorGuard::~StyleColorGuard()
        {
            ImGui::PopStyleColor(_count);
        }
        //--------------------------------------------------------------------------


        StyleVarGuard::StyleVarGuard(std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables)
            : _count(static_cast<int>(variables.size()))
        {
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
            ImGui::PopStyleVar(_count);
        }
        //--------------------------------------------------------------------------


        IDGuard::IDGuard(int id)
            : _id(id)
        {
            ImGui::PushID(_id);
        }
        //--------------------------------------------------------------------------

        IDGuard::~IDGuard()
        {
            ImGui::PopID();
        }
        //--------------------------------------------------------------------------


        GroupGuard::GroupGuard()
        {
            ImGui::BeginGroup();
        }
        //--------------------------------------------------------------------------

        GroupGuard::~GroupGuard()
        {
            ImGui::EndGroup();
        }
        //--------------------------------------------------------------------------


        void SetItemTooltip(const std::string& text, ImGuiHoveredFlags_ flags)
        {
            if (ImGui::IsItemHovered(flags))
            {
                ImGui::SetTooltip(text.c_str());
            }
        }
        //--------------------------------------------------------------------------
    }
}