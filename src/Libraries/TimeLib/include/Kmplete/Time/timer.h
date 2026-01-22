#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Time/clock.h"


namespace Kmplete
{
    namespace Time
    {
        //! Wrapper for a clock with a timeout, doesn't do anything itself
        //! when timeout is reached
        class Timer : public Clock
        {
        public:
            KMP_API explicit Timer(unsigned int timeoutMs = 0) noexcept;
            ~Timer() = default;

            KMP_API void SetTimeout(unsigned int timeoutMs, bool restart = true) noexcept;
            KMP_NODISCARD KMP_API unsigned int GetTimeout() const noexcept;

            KMP_NODISCARD KMP_API bool ReachedTimeout() const noexcept;

        protected:
            unsigned int _timeoutMs;
        };
        //--------------------------------------------------------------------------
    }
}