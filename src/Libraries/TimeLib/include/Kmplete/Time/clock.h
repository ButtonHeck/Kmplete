#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <chrono>


namespace Kmplete
{
    namespace Time
    {
        //! A simple clock object for calculating difference between timestamps
        //! in milliseconds
        class Clock
        {
        public:
            KMP_API Clock() noexcept;
            ~Clock() = default;

            KMP_API float Mark() noexcept;
            KMP_NODISCARD KMP_API float Peek() const noexcept;

        protected:
            std::chrono::high_resolution_clock::time_point _timePoint;
        };
        //--------------------------------------------------------------------------
    }
}