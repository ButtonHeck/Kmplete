#pragma once

#if defined (KMP_LOG_DISABLED) || defined (KMP_PRODUCTION_BUILD)

#define KMP_LOG_CORE_TRACE(...)
#define KMP_LOG_CORE_DEBUG(...)
#define KMP_LOG_CORE_INFO(...)
#define KMP_LOG_CORE_WARN(...)
#define KMP_LOG_CORE_ERROR(...)
#define KMP_LOG_CORE_CRITICAL(...)

#define KMP_LOG_CLIENT_TRACE(...)
#define KMP_LOG_CLIENT_DEBUG(...)
#define KMP_LOG_CLIENT_INFO(...)
#define KMP_LOG_CLIENT_WARN(...)
#define KMP_LOG_CLIENT_ERROR(...)
#define KMP_LOG_CLIENT_CRITICAL(...)

#else
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/pointers.h"

#if defined (KMP_COMPILER_MSVC)
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
#include <spdlog/spdlog.h>
#if defined (KMP_COMPILER_MSVC)
    #pragma warning(pop)
#endif

#include <sstream>

namespace Kmplete
{
    class Settings;

    class Log
    {
    public:
        KMP_API static void Boot();
        KMP_API static void Initialize();
        KMP_API static void Finalize();

        KMP_NODISCARD KMP_API static std::string_view StringLogOutput();

        KMP_API static void SaveSettings(Settings& settings);
        KMP_API static void LoadSettings(Settings& settings);

        // Core log functions
        template <typename... Args>
        static void CoreTrace(spdlog::format_string_t<Args...> fmt, Args&&... args) { _coreLogger->trace(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreDebug(spdlog::format_string_t<Args...> fmt, Args&&... args) { _coreLogger->debug(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreInfo(spdlog::format_string_t<Args...> fmt, Args&&... args) { _coreLogger->info(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreWarn(spdlog::format_string_t<Args...> fmt, Args&&... args) { _coreLogger->warn(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreError(spdlog::format_string_t<Args...> fmt, Args&&... args) { _coreLogger->error(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreCritical(spdlog::format_string_t<Args...> fmt, Args&&... args) { _coreLogger->critical(fmt, std::forward<Args>(args)...); }

        // Client log functions
        template <typename... Args>
        static void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args) { _clientLogger->trace(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args) { _clientLogger->debug(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) { _clientLogger->info(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) { _clientLogger->warn(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) { _clientLogger->error(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) { _clientLogger->critical(fmt, std::forward<Args>(args)...); }

    private:
        struct LogSettings
        {
            std::string filename = "Kmplete_log.txt";
            bool enabled = true;
            bool truncate = false;
            bool outputConsole = true;
            bool outputFile = true;
            bool outputStringBuffer = false;
            int coreLevel = spdlog::level::trace;
            int coreLevelFlush = spdlog::level::trace;
            int clientLevel = spdlog::level::trace;
            int clientLevelFlush = spdlog::level::trace;
        };

    private:
        static LogSettings _logSettings;
        static Ptr<spdlog::logger> _coreLogger;
        KMP_API static Ptr<spdlog::logger> _clientLogger;
        static std::stringstream _stringStream;
    };
    //--------------------------------------------------------------------------
}

#define KMP_LOG_CORE_TRACE(...)         ::Kmplete::Log::CoreTrace(__VA_ARGS__)
#define KMP_LOG_CORE_DEBUG(...)         ::Kmplete::Log::CoreDebug(__VA_ARGS__)
#define KMP_LOG_CORE_INFO(...)          ::Kmplete::Log::CoreInfo(__VA_ARGS__)
#define KMP_LOG_CORE_WARN(...)          ::Kmplete::Log::CoreWarn(__VA_ARGS__)
#define KMP_LOG_CORE_ERROR(...)         ::Kmplete::Log::CoreError(__VA_ARGS__)
#define KMP_LOG_CORE_CRITICAL(...)      ::Kmplete::Log::CoreCritical(__VA_ARGS__)

#define KMP_LOG_CLIENT_TRACE(...)       ::Kmplete::Log::Trace(__VA_ARGS__)
#define KMP_LOG_CLIENT_DEBUG(...)       ::Kmplete::Log::Debug(__VA_ARGS__)
#define KMP_LOG_CLIENT_INFO(...)        ::Kmplete::Log::Info(__VA_ARGS__)
#define KMP_LOG_CLIENT_WARN(...)        ::Kmplete::Log::Warn(__VA_ARGS__)
#define KMP_LOG_CLIENT_ERROR(...)       ::Kmplete::Log::Error(__VA_ARGS__)
#define KMP_LOG_CLIENT_CRITICAL(...)    ::Kmplete::Log::Critical(__VA_ARGS__)
#endif
