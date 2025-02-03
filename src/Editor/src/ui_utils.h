#pragma once

#include <imgui.h>

#include <variant>
#include <initializer_list>
#include <string>

namespace Kmplete
{
    namespace UiUtils
    {
        class DisableGuard
        {
        public:
            explicit DisableGuard(bool condition);
            ~DisableGuard();

        private:
            const bool _condition;
        };
        //--------------------------------------------------------------------------

        class ItemWidthGuard
        {
        public:
            explicit ItemWidthGuard(float width);
            ~ItemWidthGuard();
        };
        //--------------------------------------------------------------------------

        class StyleColorGuard
        {
        public:
            explicit StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors);
            ~StyleColorGuard();

        private:
            const int _count;
        };
        //--------------------------------------------------------------------------

        class StyleVarGuard
        {
        public:
            StyleVarGuard(std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables);
            ~StyleVarGuard();

        private:
            const int _count;
        };
        //--------------------------------------------------------------------------

        class IDGuard
        {
        public:
            explicit IDGuard(int id);
            ~IDGuard();

        private:
            const int _id;
        };
        //--------------------------------------------------------------------------

        class GroupGuard
        {
        public:
            GroupGuard();
            ~GroupGuard();
        };
        //--------------------------------------------------------------------------

        void SetItemTooltip(const std::string& text, ImGuiHoveredFlags_ flags = ImGuiHoveredFlags_AllowWhenDisabled);
        //--------------------------------------------------------------------------
    }
}
