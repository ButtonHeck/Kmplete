#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Graphics/graphics_backend.h"

namespace Kmplete
{
    class Settings;

    class GraphicsBackendManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(GraphicsBackendManager)

        KMP_API GraphicsBackendManager() noexcept;

        KMP_API void SaveSettings(Settings& settings) const;
        KMP_API void LoadSettings(Settings& settings);

        KMP_NODISCARD KMP_API Ptr<GraphicsBackend> CreateBackend();

    private:
        GraphicsBackend::BackendType _type;
        Ptr<GraphicsBackend> _backend;
    };
    //--------------------------------------------------------------------------
}