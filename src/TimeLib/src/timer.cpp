#include "Kmplete/Time/timer.h"


namespace Kmplete
{
    namespace Time
    {
        Timer::Timer(unsigned int timeoutMs /*= 0*/) noexcept
            : Clock()
            , _timeoutMs(timeoutMs)
        {}
        //--------------------------------------------------------------------------

        bool Timer::ReachedTimeout() const noexcept
        {
            return Peek() >= static_cast<float>(_timeoutMs);
        }
        //--------------------------------------------------------------------------

        void Timer::SetTimeout(unsigned int timeoutMs) noexcept
        {
            if (_timeoutMs == timeoutMs)
            {
                return;
            }

            _timeoutMs = timeoutMs;
            Mark();
        }
        //--------------------------------------------------------------------------

        unsigned int Timer::GetTimeout() const noexcept
        {
            return _timeoutMs;
        }
        //--------------------------------------------------------------------------
    }
}