#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/window.h"

namespace Kmplete
{
    class WindowBackend
    {
    public:
        KMP_API KMP_NODISCARD static Ptr<WindowBackend> Create();

    public:
        KMP_API virtual ~WindowBackend() = default;

        KMP_API KMP_NODISCARD virtual bool Initialize() const = 0;
        KMP_API virtual void Finalize() const = 0;

        KMP_API KMP_NODISCARD virtual Ptr<Window> CreateWindow() const = 0;
    };
    //--------------------------------------------------------------------------
}