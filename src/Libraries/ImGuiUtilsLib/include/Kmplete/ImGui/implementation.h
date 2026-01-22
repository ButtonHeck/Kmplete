#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //! Base class of an ImGui implementation, that encapsulates common ImGui function calls, derived classes
        //! should implement more specific graphics- and window-related routines necessary for ImGui to work
        class ImGuiImplementation
        {
            KMP_DISABLE_COPY_MOVE(ImGuiImplementation)

        protected:
            static constexpr auto ConfigurationFileName = "imgui.ini";
            static constexpr auto DefaultFontSize = 18;

        public:
            KMP_NODISCARD static ImGuiImplementation* CreateImpl(void* window, const String& graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, const char* configName = ConfigurationFileName);

        public:
            virtual ~ImGuiImplementation();

            void NewFrame() const;
            void Render() const;

            bool AddFont(const BinaryBuffer& fontDataBuffer, float dpiScale, int fontSize = DefaultFontSize) const;
            bool AddFont(const Filepath& fontFilepath, float dpiScale, int fontSize = DefaultFontSize) const;
            bool AddIconsFont(const BinaryBuffer& fontDataBuffer, float dpiScale, int fontSize = DefaultFontSize) const;
            bool AddIconsFont(const Filepath& fontFilepath, float dpiScale, int fontSize = DefaultFontSize) const;

            void Stylize(float dpiScale) const;

            virtual void CreateFontsTexture() const = 0;

        protected:
            ImGuiImplementation(bool dockingEnabled, bool viewportsEnabled, const char* configName = ConfigurationFileName);

            virtual void _NewFrameImpl() const = 0;
            virtual void _RenderImpl() const = 0;
        };
        //--------------------------------------------------------------------------
    }
}