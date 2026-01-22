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
    //! Global logging controller backed by the spdlog library, available for logging
    //! to a console, a file or a string stream. Initialization is split to two parts:
    //! first by a "Boot" function that stores all messages in a temporary storage, then
    //! when an application loads its settings (including logger settings) all messages
    //! from that storage moved to sinks according to those settings.
    //! Filtering of messages is done by a levels mechanism (from 0 to 5): level 0 keeps
    //! all the messages while level 5 keeps only critical ones.
    class Log
    {
    public:
        static constexpr auto SettingsEntryName = "Log";
        static constexpr auto FilenameStr = "Filename";
        static constexpr auto EnabledStr = "Enabled";
        static constexpr auto TruncateStr = "Truncate";
        static constexpr auto OutputConsoleStr = "OutputConsole";
        static constexpr auto OutputFileStr = "OutputFile";
        static constexpr auto OutputStringBufferStr = "OutputStringBuffer";
        static constexpr auto LevelStr = "Level";
        static constexpr auto LevelFlushStr = "LevelFlush";

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
        KMP_LOG_API static void Initialize(const String& programName);
        KMP_LOG_API static void Finalize();

        KMP_LOG_API static void SetSettings(const LogSettings& settings);
        KMP_LOG_API static const LogSettings& GetSettings();

        KMP_NODISCARD KMP_LOG_API static std::stringstream& StringLogOutput();

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
        KMP_LOG_API static Ptr<spdlog::logger> _logger;
    };
    //--------------------------------------------------------------------------
}

//! Utility struct that enables logging Filepath without explicitly converting it to a string
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


//! Two sets of logging macros for all levels: the first set is used for a class functions (with class name printed first),
//! while the second one is used for any other parts of code
//! @see log_class_macro.h

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
