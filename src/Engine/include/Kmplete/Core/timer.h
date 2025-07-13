#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <chrono>

namespace Kmplete
{
    class Timer
    {
    public:
        KMP_API explicit Timer(unsigned int timeoutMs = 0) noexcept;
        ~Timer() = default;

        KMP_API float Mark() noexcept;
        KMP_NODISCARD KMP_API float Peek() const noexcept;

        KMP_API void SetTimeout(unsigned int timeoutMs) noexcept;
        KMP_NODISCARD KMP_API unsigned int GetTimeout() const noexcept;

        KMP_NODISCARD KMP_API bool ReachedTimeout() const noexcept;

    protected:
        unsigned int _timeoutMs;
        std::chrono::high_resolution_clock::time_point _last;
    };
    //--------------------------------------------------------------------------
}