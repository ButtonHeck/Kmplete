#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        class ImGuiImplementation
        {
        public:
            KMP_DISABLE_COPY_MOVE(ImGuiImplementation)

            ImGuiImplementation() = default;
            virtual ~ImGuiImplementation() = default;

            virtual void NewFrame() = 0;
            virtual void Render() = 0;
        };
        //--------------------------------------------------------------------------
    }
}