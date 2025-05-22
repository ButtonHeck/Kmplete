#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/texture_manager.h"

namespace Kmplete
{
    class GraphicsBackend
    {
    public:
        KMP_NODISCARD KMP_API static UPtr<GraphicsBackend> Create(GraphicsBackendType type);

    public:
        KMP_DISABLE_COPY_MOVE(GraphicsBackend)

        KMP_API explicit GraphicsBackend(GraphicsBackendType type);
        virtual ~GraphicsBackend() = default;

        KMP_API virtual void Initialize() const = 0;

        KMP_NODISCARD KMP_API GraphicsBackendType GetType() const noexcept;
        KMP_NODISCARD KMP_API TextureManager& GetTextureManager();

    protected:
        GraphicsBackendType _type;
        UPtr<TextureManager> _textureManager;
    };
    //--------------------------------------------------------------------------
}