#include "Kmplete/Time/clock.h"


namespace Kmplete
{
    namespace Time
    {
        Clock::Clock() noexcept
            : _timePoint(std::chrono::high_resolution_clock::now())
        {}
        //--------------------------------------------------------------------------

        float Clock::Mark() noexcept
        {
            const auto old = _timePoint;
            _timePoint = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<float, std::milli> timeElapsed = _timePoint - old;
            return timeElapsed.count();
        }
        //--------------------------------------------------------------------------

        float Clock::Peek() const noexcept
        {
            return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - _timePoint).count();
        }
        //--------------------------------------------------------------------------
    }
}