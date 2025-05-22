#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <chrono>

namespace Kmplete
{
    class Timer
    {
    public:
        KMP_API explicit Timer(unsigned int timeoutMs) noexcept;

        KMP_API float Mark() noexcept;
        KMP_NODISCARD KMP_API float Peek() const noexcept;

        KMP_API void SetTimeout(unsigned int timeoutMs) noexcept;
        KMP_NODISCARD KMP_API unsigned int GetTimeout() const noexcept;

        KMP_NODISCARD KMP_API bool ReachedTimeout() const noexcept;

    private:
        unsigned int _timeoutMs;
        std::chrono::steady_clock::time_point _last;
    };
    //--------------------------------------------------------------------------
}