#pragma once

#include <imgui.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        void InitializeImGui(bool dockingEnabled, bool viewportsEnabled);
        void SetItemTooltip(const char* text, ImGuiHoveredFlags_ flags = ImGuiHoveredFlags_AllowWhenDisabled);
    }
}
