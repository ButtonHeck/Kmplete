#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/ImGui/context.h"

#include <imgui.h>


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
            static constexpr auto DefaultFontSize = 18;

        public:
            KMP_NODISCARD static ImGuiImplementation* CreateImpl(Context* implementationContext);

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
            virtual void AddTexture(StringID sid, void* sampler, void* view) = 0;
            virtual void RemoveTexture(StringID sid) = 0;
            KMP_NODISCARD ImTextureID GetTexture(StringID sid) const;

        protected:
            explicit ImGuiImplementation(Context* implementationContext);

            virtual void _NewFrameImpl() const = 0;
            virtual void _RenderImpl() const = 0;

        protected:
            UPtr<Context> _context;
            HashMap<StringID, ImTextureID> _textureMap;
        };
        //--------------------------------------------------------------------------
    }
}