#pragma once

#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //! Base class of an ImGUI instance properties and settings
        struct Context
        {
            static constexpr auto ConfigurationFileName = "imgui.ini";

            Context(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, float baseScale);
            virtual ~Context() = default;

            void* window;
            String graphicsBackendType;
            bool dockingEnabled;
            bool viewportsEnabled;
            const char* configName = ConfigurationFileName;
            const float baseScale;
        };
        //--------------------------------------------------------------------------
    }
}