#pragma once

#if defined (KMP_CONFIG_TYPE_PRODUCTION)

#define KMP_LOG_TRACE(...)
#define KMP_LOG_DEBUG(...)
#define KMP_LOG_INFO(...)
#define KMP_LOG_WARN(...)
#define KMP_LOG_ERROR(...)
#define KMP_LOG_CRITICAL(...)

#define KMP_LOG_TRACE_FN(...)
#define KMP_LOG_DEBUG_FN(...)
#define KMP_LOG_INFO_FN(...)
#define KMP_LOG_WARN_FN(...)
#define KMP_LOG_ERROR_FN(...)
#define KMP_LOG_CRITICAL_FN(...)

#else
#include "Kmplete/Log/log_api.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"

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
    class Log
    {
    public:
        constexpr static auto SettingsEntryName = "Log";
        constexpr static auto FilenameStr = "Filename";
        constexpr static auto EnabledStr = "Enabled";
        constexpr static auto TruncateStr = "Truncate";
        constexpr static auto OutputConsoleStr = "OutputConsole";
        constexpr static auto OutputFileStr = "OutputFile";
        constexpr static auto OutputStringBufferStr = "OutputStringBuffer";
        constexpr static auto LevelStr = "Level";
        constexpr static auto LevelFlushStr = "LevelFlush";

        struct LogSettings
        {
            String filename = "Kmplete_log.txt";
            bool enabled = true;
            bool truncate = false;
            bool outputConsole = true;
            bool outputFile = true;
            bool outputStringBuffer = false;
            int level = spdlog::level::trace;
            int levelFlush = spdlog::level::trace;
        };

    public:
        KMP_LOG_API static void Boot(const String& programName);
        KMP_LOG_API static void Initialize(const LogSettings& settings, const String& programName);
        KMP_LOG_API static void Finalize();

        KMP_NODISCARD KMP_LOG_API static std::string_view StringLogOutput();

        template <typename... Args>
        static void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_logger) _logger->trace(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_logger) _logger->debug(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_logger) _logger->info(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_logger) _logger->warn(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_logger) _logger->error(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_logger) _logger->critical(fmt, std::forward<Args>(args)...); }

    private:
        static LogSettings _logSettings;
        KMP_LOG_API static Ptr<spdlog::logger> _logger;
        static std::stringstream _stringStream;
    };
    //--------------------------------------------------------------------------
}

template<>
struct fmt::formatter<Kmplete::Filepath> : fmt::formatter<Kmplete::String>
{
    template <typename FormatContext>
    auto format(const Kmplete::Filepath& filepath, FormatContext& ctx)
    {
        return fmt::formatter<Kmplete::String>::format(filepath.generic_u8string(), ctx);
    }
};
//--------------------------------------------------------------------------

#define KMP_LOG_TRACE(...)          ::Kmplete::Log::Trace("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_DEBUG(...)          ::Kmplete::Log::Debug("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_INFO(...)           ::Kmplete::Log::Info("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_WARN(...)           ::Kmplete::Log::Warn("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_ERROR(...)          ::Kmplete::Log::Error("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_CRITICAL(...)       ::Kmplete::Log::Critical("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))

#define KMP_LOG_TRACE_FN(...)       ::Kmplete::Log::Trace(__VA_ARGS__)
#define KMP_LOG_DEBUG_FN(...)       ::Kmplete::Log::Debug(__VA_ARGS__)
#define KMP_LOG_INFO_FN(...)        ::Kmplete::Log::Info(__VA_ARGS__)
#define KMP_LOG_WARN_FN(...)        ::Kmplete::Log::Warn(__VA_ARGS__)
#define KMP_LOG_ERROR_FN(...)       ::Kmplete::Log::Error(__VA_ARGS__)
#define KMP_LOG_CRITICAL_FN(...)    ::Kmplete::Log::Critical(__VA_ARGS__)

#endif
