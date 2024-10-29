#include "Kmplete/Core/log.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/string_utils.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>

#include <chrono>
#if defined (KMP_PLATFORM_LINUX) || defined (KMP_COMPILER_MINGW)
#include <iomanip>
#endif

namespace Kmplete
{
    constexpr static auto LogSettingsEntryName = "Log";
    constexpr static auto LogFilenameStr = "Filename";
    constexpr static auto LogEnabledStr = "Enabled";
    constexpr static auto LogTruncateStr = "Truncate";
    constexpr static auto LogOutputConsoleStr = "OutputConsole";
    constexpr static auto LogOutputFileStr = "OutputFile";
    constexpr static auto LogOutputStringBufferStr = "OutputStringBuffer";
    constexpr static auto LogCoreLevelStr = "CoreLevel";
    constexpr static auto LogClientLevelStr = "ClientLevel";

    Log::LogSettings Log::_logSettings;
    Ptr<spdlog::logger> Log::_coreLogger;
    Ptr<spdlog::logger> Log::_clientLogger;
    std::stringstream Log::_stringStream;

    void Log::InitializeTemporarySink()
    {
        const auto stringBufferLog = CreatePtr<spdlog::sinks::ostream_sink_mt>(_stringStream);
        stringBufferLog->set_pattern("%v");

        _coreLogger = CreatePtr<spdlog::logger>("CORE", stringBufferLog);
        _clientLogger = CreatePtr<spdlog::logger>("CLIENT", stringBufferLog);

        _coreLogger->set_level(spdlog::level::trace);
        _coreLogger->flush_on(spdlog::level::trace);
        _clientLogger->set_level(spdlog::level::trace);
        _clientLogger->flush_on(spdlog::level::trace);

        spdlog::register_logger(_coreLogger);
        spdlog::register_logger(_clientLogger);
    }
    //--------------------------------------------------------------------------

    void Log::Initialize()
    {
        auto temporaryLogBuffer = _stringStream.str();
        _stringStream.clear();

        spdlog::drop_all();

        if (_logSettings.enabled)
        {
            std::vector<spdlog::sink_ptr> logSinks;

            if (_logSettings.outputConsole)
            {
                const auto stdoutLog = CreatePtr<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::automatic);
                stdoutLog->set_pattern("%^[%T.%e] [%l] %n: %v%$");
                logSinks.push_back(stdoutLog);
            }

            if (_logSettings.outputFile)
            {
                const auto fileLog = CreatePtr<spdlog::sinks::basic_file_sink_mt>(_logSettings.filename, _logSettings.truncate);
                fileLog->set_pattern("[%T.%e] [%l] %n: %v");
                logSinks.push_back(fileLog);
            }

            if (_logSettings.outputStringBuffer)
            {
                const auto stringBufferLog = CreatePtr<spdlog::sinks::ostream_sink_mt>(_stringStream);
                stringBufferLog->set_pattern("[%T.%e] [%l] %n: %v");
                logSinks.push_back(stringBufferLog);
            }

            _coreLogger = CreatePtr<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
            _clientLogger = CreatePtr<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));

            const auto coreLevel = static_cast<spdlog::level::level_enum>(std::clamp(_logSettings.coreLevel, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));
            const auto clientLevel = static_cast<spdlog::level::level_enum>(std::clamp(_logSettings.clientLevel, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));

            _coreLogger->set_level(coreLevel);
            _coreLogger->flush_on(coreLevel);
            _clientLogger->set_level(clientLevel);
            _clientLogger->flush_on(clientLevel);
        }
        else
        {
            const auto nullSink = CreatePtr<spdlog::sinks::null_sink_mt>();
            _coreLogger = CreatePtr<spdlog::logger>("CORE", nullSink);
            _clientLogger = CreatePtr<spdlog::logger>("CLIENT", nullSink);
        }        

        spdlog::register_logger(_coreLogger);
        spdlog::register_logger(_clientLogger);

        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        Log::CoreInfo("-------{}", Utils::Concatenate(std::put_time(localtime(&now), "%F %T")));

        if (!temporaryLogBuffer.empty())
        {
            temporaryLogBuffer.pop_back();
            Log::CoreInfo("{}", temporaryLogBuffer);
        }
    }
    //--------------------------------------------------------------------------

    void Log::Finalize()
    {
        _clientLogger->flush();
        _coreLogger->flush();
        spdlog::drop_all();
    }
    //--------------------------------------------------------------------------

    std::string_view Log::StringLogOutput()
    {
        return _stringStream.view();
    }
    //--------------------------------------------------------------------------

    void Log::SaveSettings(Settings& settings)
    {
#if !defined (KMP_LOG_DISABLED)
        settings.StartSaveObject(LogSettingsEntryName);
        settings.SaveString(LogFilenameStr, Utils::NarrowToUtf8(_logSettings.filename));
        settings.SaveBool(LogEnabledStr, _logSettings.enabled);
        settings.SaveBool(LogTruncateStr, _logSettings.truncate);
        settings.SaveBool(LogOutputConsoleStr, _logSettings.outputConsole);
        settings.SaveBool(LogOutputFileStr, _logSettings.outputFile);
        settings.SaveBool(LogOutputStringBufferStr, _logSettings.outputStringBuffer);
        settings.SaveInt(LogCoreLevelStr, _logSettings.coreLevel);
        settings.SaveInt(LogClientLevelStr, _logSettings.clientLevel);
        settings.EndSaveObject();
#else
        (void)settings;
#endif
    }
    //--------------------------------------------------------------------------

    void Log::LoadSettings(Settings& settings)
    {
#if !defined (KMP_LOG_DISABLED)
        settings.StartLoadObject(LogSettingsEntryName);
        _logSettings.filename = Utils::Utf8ToNarrow(settings.GetString(LogFilenameStr, "Kmplete_log.txt"));
        _logSettings.enabled = settings.GetBool(LogEnabledStr, true);
        _logSettings.truncate = settings.GetBool(LogTruncateStr, false);
        _logSettings.outputConsole = settings.GetBool(LogOutputConsoleStr, true);
        _logSettings.outputFile = settings.GetBool(LogOutputFileStr, true);
        _logSettings.outputStringBuffer = settings.GetBool(LogOutputStringBufferStr, false);
        _logSettings.coreLevel = settings.GetInt(LogCoreLevelStr, spdlog::level::trace);
        _logSettings.clientLevel = settings.GetInt(LogClientLevelStr, spdlog::level::trace);
        settings.EndLoadObject();
#else
        (void)settings;
#endif
    }
    //--------------------------------------------------------------------------
}
