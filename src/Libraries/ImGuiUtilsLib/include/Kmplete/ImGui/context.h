#pragma once

#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //TODO: comments
        struct Context
        {
            static constexpr auto ConfigurationFileName = "imgui.ini";

            Context(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled);
            virtual ~Context() = default;

            void* window;
            String graphicsBackendType;
            bool dockingEnabled;
            bool viewportsEnabled;
            const char* configName = ConfigurationFileName;
        };
        //--------------------------------------------------------------------------
    }
}