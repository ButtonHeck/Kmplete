#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/graphics_base.h"


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

    protected:
        GraphicsBackendType _type;
    };
    //--------------------------------------------------------------------------
}