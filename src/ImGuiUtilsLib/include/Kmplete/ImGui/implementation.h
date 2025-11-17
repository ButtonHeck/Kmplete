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
            KMP_NODISCARD static ImGuiImplementation* CreateImpl(void* window, const String& graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, const char* configName = "imgui.ini");

        public:
            KMP_DISABLE_COPY_MOVE(ImGuiImplementation)

            virtual ~ImGuiImplementation();

            void NewFrame() const;
            void Render() const;

            void AddFont(const BinaryBuffer& fontDataBuffer, float dpiScale) const;
            void AddFont(const Filepath& fontFilepath, float dpiScale) const;
            void AddIconsFont(const BinaryBuffer& fontDataBuffer, float dpiScale) const;
            void AddIconsFont(const Filepath& fontFilepath, float dpiScale) const;

            void Stylize(float dpiScale) const;

            virtual void CreateFontsTexture() const = 0;

        protected:
            ImGuiImplementation(bool dockingEnabled, bool viewportsEnabled, const char* configName = "imgui.ini");

            virtual void _NewFrameImpl() const = 0;
            virtual void _RenderImpl() const = 0;
        };
        //--------------------------------------------------------------------------
    }
}