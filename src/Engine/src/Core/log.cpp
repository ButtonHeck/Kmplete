#include "Kmplete/Core/log.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/string_utils.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <chrono>
#if defined KMPLETE_PLATFORM_LINUX || defined KMPLETE_COMPILER_MINGW
#include <iomanip>
#endif

namespace Kmplete
{
    void LogSettings::SaveSettings(const Ptr<Settings> settings) const
    {
        settings->StartSaveGroup("Log");

        settings->SaveString("Filename", filename);
        settings->SaveBool("Enabled", enabled);
        settings->SaveBool("Truncate", truncate);
        settings->SaveBool("OutputConsole", outputConsole);
        settings->SaveBool("OutputFile", outputFile);
        settings->SaveBool("OutputStringBuffer", outputStringBuffer);
        settings->SaveInt("CoreLevel", coreLevel);
        settings->SaveInt("ClientLevel", clientLevel);

        settings->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    void LogSettings::LoadSettings(const Ptr<Settings> settings)
    {
        settings->StartLoadGroup("Log");

        filename = settings->GetString("Filename", "Kmplete_log.txt");
        enabled = settings->GetBool("Enabled", true);
        truncate = settings->GetBool("Truncate", false);
        outputConsole = settings->GetBool("OutputConsole", true);
        outputFile = settings->GetBool("OutputFile", true);
        outputStringBuffer = settings->GetBool("OutputStringBuffer", false);
        coreLevel = settings->GetInt("CoreLevel", spdlog::level::trace);
        clientLevel = settings->GetInt("ClientLevel", spdlog::level::trace);

        settings->EndLoadGroup();
    }
    //--------------------------------------------------------------------------


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

    void Log::Initialize(const LogSettings& settings)
    {
        const auto temporaryLogBuffer = _stringStream.str();
        _stringStream.clear();

        spdlog::drop_all();

        std::vector<spdlog::sink_ptr> logSinks;

        if (settings.outputConsole)
        {
            const auto stdoutLog = CreatePtr<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::automatic);
            stdoutLog->set_pattern("%^[%T.%e] [%l] %n: %v%$");
            logSinks.push_back(stdoutLog);
        }

        if (settings.outputFile)
        {
            const auto fileLog = CreatePtr<spdlog::sinks::basic_file_sink_mt>(settings.filename, settings.truncate);
            fileLog->set_pattern("[%T.%e] [%l] %n: %v");
            logSinks.push_back(fileLog);
        }

        if (settings.outputStringBuffer)
        {
            const auto stringBufferLog = CreatePtr<spdlog::sinks::ostream_sink_mt>(_stringStream);
            stringBufferLog->set_pattern("[%T.%e] [%l] %n: %v");
            logSinks.push_back(stringBufferLog);
        }

        _coreLogger = CreatePtr<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        _clientLogger = CreatePtr<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));

        //todo: simplify logging to empty sink
        if (settings.enabled)
        {
            const auto coreLevel = static_cast<spdlog::level::level_enum>(std::clamp(settings.coreLevel, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));
            const auto clientLevel = static_cast<spdlog::level::level_enum>(std::clamp(settings.clientLevel, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));

            _coreLogger->set_level(coreLevel);
            _coreLogger->flush_on(coreLevel);
            _clientLogger->set_level(clientLevel);
            _clientLogger->flush_on(clientLevel);
        }
        else
        {
            _coreLogger->set_level(spdlog::level::off);
            _clientLogger->set_level(spdlog::level::off);
        }

        spdlog::register_logger(_coreLogger);
        spdlog::register_logger(_clientLogger);

        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        Log::CoreInfo("-------{}", Utils::Concatenate(std::put_time(localtime(&now), "%F %T")));
        Log::CoreInfo("{}", temporaryLogBuffer);
    }
    //--------------------------------------------------------------------------

    Ptr<spdlog::logger>& Log::CoreLogger()
    {
        return _coreLogger;
    }
    //--------------------------------------------------------------------------

    Ptr<spdlog::logger>& Log::ClientLogger()
    {
        return _clientLogger;
    }
    //--------------------------------------------------------------------------

    std::string_view Log::StringLogOutput()
    {
        return _stringStream.view();
    }
    //--------------------------------------------------------------------------
}
