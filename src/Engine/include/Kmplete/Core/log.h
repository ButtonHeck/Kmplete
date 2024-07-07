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
    class Settings;

    struct KMPLETE_API LogSettings
    {
        std::string filename = "Kmplete_log.txt";
        bool enabled = true;
        bool truncate = false;
        bool outputConsole = true;
        bool outputFile = true;
        bool outputStringBuffer = false;
        int coreLevel = spdlog::level::trace;
        int clientLevel = spdlog::level::trace;

        void SaveSettings(const Ptr<Settings> settings) const;
        void LoadSettings(const Ptr<Settings> settings);
    };
    //--------------------------------------------------------------------------


    class KMPLETE_API Log
    {
    public:
        static void Initialize(const LogSettings& settings);

        static Ptr<spdlog::logger>& CoreLogger();
        static Ptr<spdlog::logger>& ClientLogger();
        static std::string_view StringLogOutput();

        // Core log functions
        template <typename... Args>
        static void CoreTrace(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_coreLogger) _coreLogger->trace(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreDebug(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_coreLogger) _coreLogger->debug(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreInfo(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_coreLogger) _coreLogger->info(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreWarn(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_coreLogger) _coreLogger->warn(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreError(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_coreLogger) _coreLogger->error(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void CoreCritical(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_coreLogger) _coreLogger->critical(fmt, std::forward<Args>(args)...); }

        // Client log functions
        template <typename... Args>
        static void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_clientLogger) _clientLogger->trace(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_clientLogger) _clientLogger->debug(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_clientLogger) _clientLogger->info(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_clientLogger) _clientLogger->warn(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_clientLogger) _clientLogger->error(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) { if (_clientLogger) _clientLogger->critical(fmt, std::forward<Args>(args)...); }

    private:
        static Ptr<spdlog::logger> _coreLogger;
        static Ptr<spdlog::logger> _clientLogger;
        static std::stringstream _ss;
    };
    //--------------------------------------------------------------------------
}
