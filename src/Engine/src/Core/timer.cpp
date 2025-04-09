#include "Kmplete/Core/timer.h"

namespace Kmplete
{
    Timer::Timer(unsigned int timeoutMs) noexcept
        : _timeoutMs(timeoutMs)
        , _last(std::chrono::steady_clock::now())
    {}
    //--------------------------------------------------------------------------

    float Timer::Mark() noexcept
    {
        const auto old = _last;
        _last = std::chrono::steady_clock::now();
        const std::chrono::duration<float, std::milli> frameTime = _last - old;
        return frameTime.count();
    }
    //--------------------------------------------------------------------------

    float Timer::Peek() const noexcept
    {
        return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - _last).count();
    }
    //--------------------------------------------------------------------------

    bool Timer::ReachedTimeout() const noexcept
    {
        return Peek() >= static_cast<float>(_timeoutMs);
    }
    //--------------------------------------------------------------------------

    void Timer::SetTimeout(unsigned int timeoutMs) noexcept
    {
        _timeoutMs = timeoutMs;
    }
    //--------------------------------------------------------------------------

    unsigned int Timer::GetTimeout() const noexcept
    {
        return _timeoutMs;
    }
    //--------------------------------------------------------------------------
}