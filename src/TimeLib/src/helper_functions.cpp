#include "Kmplete/Time/helper_functions.h"
#include "Kmplete/Base/platform.h"

#include <chrono>
#include <iomanip>


namespace Kmplete
{
    namespace Time
    {
        String GetCurrentTimeString(const char* format /*= "%F %T"*/)
        {
            const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::stringstream timeStream;

#if defined (KMP_COMPILER_MSVC)
            struct tm buf{};
            localtime_s(&buf, &now);
            timeStream << std::put_time(&buf, format);
#else
            timeStream << std::put_time(localtime(&now), format);
#endif

            return timeStream.str();
        }
        //--------------------------------------------------------------------------
    }
}