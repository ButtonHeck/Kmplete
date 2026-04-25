#pragma once

#include "Kmplete/Base/types_aliases.h"

#include <imgui.h>

#include <variant>


namespace Kmplete
{
    //! Syntactic sugar helper structs that merges some common ImGui paired calls 
    //! like BeginSomething/EndSomething into a single object
    namespace ImGuiUtils
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
            explicit StyleColorGuard(InitializerList<Pair<ImGuiCol_, ImVec4>>&& colors);
            ~StyleColorGuard();

        private:
            const int _count;
        };
        //--------------------------------------------------------------------------

        class StyleVarGuard
        {
        public:
            StyleVarGuard(InitializerList<Pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables);
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
    }
}