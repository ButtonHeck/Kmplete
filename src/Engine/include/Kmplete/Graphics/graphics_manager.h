#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/texture_manager.h"

namespace Kmplete
{
    class Settings;

    class GraphicsManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(GraphicsManager)

        KMP_API GraphicsManager() noexcept;

        KMP_API void SaveSettings(Settings& settings) const;
        KMP_API void LoadSettings(Settings& settings);

        KMP_NODISCARD KMP_API bool CreateBackend();
        KMP_NODISCARD KMP_API GraphicsBackend& GetBackend();

        KMP_NODISCARD KMP_API bool CreateTextureManager();
        KMP_NODISCARD KMP_API TextureManager& GetTextureManager();

    private:
        GraphicsBackend::BackendType _type;
        UPtr<GraphicsBackend> _backend;
        UPtr<TextureManager> _textureManager;
    };
    //--------------------------------------------------------------------------
}