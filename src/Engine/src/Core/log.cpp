#include "Kmplete/Core/log.h"
#include "Kmplete/Core/platform.h"
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
    Ptr<spdlog::logger> Log::_coreLogger;
    Ptr<spdlog::logger> Log::_clientLogger;
    std::stringstream Log::_ss;

    void Log::Initialize(const LogConfig& config)
    {
        std::vector<spdlog::sink_ptr> logSinks;

        if (config.outputConsole)
        {
            const auto stdoutLog = CreatePtr<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::automatic);
            stdoutLog->set_pattern("%^[%T.%e] [%l] %n: %v%$");
            logSinks.push_back(stdoutLog);
        }

        if (config.outputFile)
        {
            const auto fileLog = CreatePtr<spdlog::sinks::basic_file_sink_mt>(config.filename, config.truncate);
            fileLog->set_pattern("[%T.%e] [%l] %n: %v");
            logSinks.push_back(fileLog);
        }

        if (config.outputStringBuffer)
        {
            const auto stringBufferLog = CreatePtr<spdlog::sinks::ostream_sink_mt>(_ss);
            stringBufferLog->set_pattern("[%T.%e] [%l] %n: %v");
            logSinks.push_back(stringBufferLog);
        }

        _coreLogger = CreatePtr<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        _clientLogger = CreatePtr<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));

        if (config.enabled)
        {
            const auto coreLevel = static_cast<spdlog::level::level_enum>(std::clamp(config.coreLevel, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));
            const auto clientLevel = static_cast<spdlog::level::level_enum>(std::clamp(config.clientLevel, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));

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
        KMPLETE_CORE_LOG_INFO("-------{}", Utils::Concatenate(std::put_time(localtime(&now), "%F %T")));
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
        return _ss.view();
    }
    //--------------------------------------------------------------------------
}
