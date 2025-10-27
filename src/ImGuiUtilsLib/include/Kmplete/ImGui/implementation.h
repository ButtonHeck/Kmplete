#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        class ImGuiImplementation
        {
        public:
            KMP_NODISCARD static ImGuiImplementation* CreateImpl(void* window, const String& graphicsBackendType);

        public:
            KMP_DISABLE_COPY_MOVE(ImGuiImplementation)

            virtual ~ImGuiImplementation() = default;

            void NewFrame() const;
            void Render() const;

        protected:
            ImGuiImplementation() = default;

            virtual void NewFrameImpl() const = 0;
            virtual void RenderImpl() const = 0;
        };
        //--------------------------------------------------------------------------
    }
}