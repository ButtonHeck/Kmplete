#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Time/clock.h"


namespace Kmplete
{
    namespace Time
    {
        //! Wrapper for a clock with a timeout, doesn't do anything itself
        //! when timeout is reached
        class KMP_API Timer : public Clock
        {
        public:
            explicit Timer(unsigned int timeoutMs = 0) noexcept;
            ~Timer() = default;

            void SetTimeout(unsigned int timeoutMs, bool restart = true) noexcept;
            KMP_NODISCARD unsigned int GetTimeout() const noexcept;

            KMP_NODISCARD bool ReachedTimeout() const noexcept;

        protected:
            unsigned int _timeoutMs;
        };
        //--------------------------------------------------------------------------
    }
}