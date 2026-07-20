#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <chrono>


namespace Kmplete
{
    namespace Time
    {
        //! A simple clock object for calculating difference between timestamps
        //! in milliseconds
        class KMP_API Clock
        {
        public:
            Clock() noexcept;
            ~Clock() = default;

            float Mark() noexcept;
            KMP_NODISCARD float Peek() const noexcept;

        protected:
            std::chrono::high_resolution_clock::time_point _timePoint;
        };
        //--------------------------------------------------------------------------
    }
}