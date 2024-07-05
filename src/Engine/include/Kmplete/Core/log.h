#pragma once

#include "kmplete_export.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/pointers.h"

#if defined (KMPLETE_COMPILER_MSVC)
    #pragma warning(disable : 4996)
#endif
#include <spdlog/spdlog.h>

#include <sstream>

namespace Kmplete
{
    struct LogConfig
    {
        std::string filename = "Kmplete.log";
        bool enabled = true;
        bool truncate = false;
        bool outputConsole = true;
        bool outputFile = true;
        bool outputStringBuffer = false;
        int coreLevel = spdlog::level::trace;
        int clientLevel = spdlog::level::trace;
    };
    //--------------------------------------------------------------------------

    class KMPLETE_API Log
    {
    public:
        static void Initialize(const LogConfig& config);

        static Ptr<spdlog::logger>& CoreLogger();
        static Ptr<spdlog::logger>& ClientLogger();
        static std::string_view StringLogOutput();

    private:
        static Ptr<spdlog::logger> _coreLogger;
        static Ptr<spdlog::logger> _clientLogger;
        static std::stringstream _ss;
    };
    //--------------------------------------------------------------------------
}

#define KMPLETE_CORE_LOG_TRACE(...)       ::Kmplete::Log::CoreLogger()->trace(__VA_ARGS__)
#define KMPLETE_CORE_LOG_DEBUG(...)       ::Kmplete::Log::CoreLogger()->debug(__VA_ARGS__)
#define KMPLETE_CORE_LOG_INFO(...)        ::Kmplete::Log::CoreLogger()->info(__VA_ARGS__)
#define KMPLETE_CORE_LOG_WARN(...)        ::Kmplete::Log::CoreLogger()->warn(__VA_ARGS__)
#define KMPLETE_CORE_LOG_ERROR(...)       ::Kmplete::Log::CoreLogger()->error(__VA_ARGS__)
#define KMPLETE_CORE_LOG_CRITICAL(...)    ::Kmplete::Log::CoreLogger()->critical(__VA_ARGS__)

#define KMPLETE_CLIENT_LOG_TRACE(...)     ::Kmplete::Log::ClientLogger()->trace(__VA_ARGS__)
#define KMPLETE_CLIENT_LOG_DEBUG(...)     ::Kmplete::Log::ClientLogger()->debug(__VA_ARGS__)
#define KMPLETE_CLIENT_LOG_INFO(...)      ::Kmplete::Log::ClientLogger()->info(__VA_ARGS__)
#define KMPLETE_CLIENT_LOG_WARN(...)      ::Kmplete::Log::ClientLogger()->warn(__VA_ARGS__)
#define KMPLETE_CLIENT_LOG_ERROR(...)     ::Kmplete::Log::ClientLogger()->error(__VA_ARGS__)
#define KMPLETE_CLIENT_LOG_CRITICAL(...)  ::Kmplete::Log::ClientLogger()->critical(__VA_ARGS__)