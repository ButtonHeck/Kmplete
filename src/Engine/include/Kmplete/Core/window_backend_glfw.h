#pragma once

#include "Kmplete/Core/window_backend.h"

namespace Kmplete
{
    class WindowBackendGlfw : public WindowBackend
    {
    public:
        KMP_API ~WindowBackendGlfw();

        KMP_API KMP_NODISCARD virtual bool Initialize() const override;
        KMP_API virtual void Finalize() const override;

        KMP_API KMP_NODISCARD virtual Ptr<Window> CreateWindow() const override;
    };
    //--------------------------------------------------------------------------
}