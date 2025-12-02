#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Time/clock.h"


namespace Kmplete
{
    namespace Time
    {
        class Timer : public Clock
        {
        public:
            KMP_API explicit Timer(unsigned int timeoutMs = 0) noexcept;
            ~Timer() = default;

            KMP_API void SetTimeout(unsigned int timeoutMs) noexcept;
            KMP_NODISCARD KMP_API unsigned int GetTimeout() const noexcept;

            KMP_NODISCARD KMP_API bool ReachedTimeout() const noexcept;

        protected:
            unsigned int _timeoutMs;
        };
        //--------------------------------------------------------------------------
    }
}