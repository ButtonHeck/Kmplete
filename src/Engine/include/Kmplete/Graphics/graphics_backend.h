#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"

#include <string>

namespace Kmplete
{
    class GraphicsBackend
    {
    public:
        constexpr static auto OpenGLStr = "OpenGL";
        constexpr static auto UnknownStr = "Unknown";
        constexpr static auto DefaultAPIStr = OpenGLStr;

    public:
        enum class BackendType
        {
            OpenGL,
            Unknown
        };

        KMP_NODISCARD static std::string BackendTypeToString(BackendType type) noexcept;
        KMP_NODISCARD static BackendType StringToBackendType(const std::string& string) noexcept;

    public:
        KMP_NODISCARD KMP_API static Ptr<GraphicsBackend> Create(BackendType type);

    public:
        KMP_DISABLE_COPY_MOVE(GraphicsBackend)

        GraphicsBackend() = default;
        virtual ~GraphicsBackend() = default;

        KMP_API virtual void Initialize() const = 0;

        KMP_NODISCARD KMP_API virtual BackendType GetType() const noexcept = 0;
    };
    //--------------------------------------------------------------------------
}