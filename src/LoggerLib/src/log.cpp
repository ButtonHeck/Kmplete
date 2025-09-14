#if !defined (KMP_CONFIG_TYPE_PRODUCTION)

#include "Kmplete/Log/log.h"

#include "spdlog/async.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/details/log_msg.h>

#include <chrono>
#if defined (KMP_PLATFORM_LINUX) || defined (KMP_COMPILER_MINGW)
    #include <iomanip>
#endif

namespace Kmplete
{
    Log::LogSettings Log::_logSettings;
    Ptr<spdlog::logger> Log::_logger;
    std::stringstream Log::_stringStream;

    namespace
    {
        struct BootMessage
        {
            String textPersistent;
            spdlog::details::log_msg logMsg;

            BootMessage(const String& text, const spdlog::details::log_msg& msg)
                : textPersistent(text)
                , logMsg(msg.logger_name, msg.level, textPersistent)
            {}
        };

        static Vector<BootMessage> bootMessages;
    }
    //--------------------------------------------------------------------------

    void Log::Boot(const String& programName)
    {
        if (_logger)
        {
            Log::Warn("Log: logger instance has already been booted");
            return;
        }

        bootMessages.reserve(64);

        const auto callbackSink = CreatePtr<spdlog::sinks::callback_sink_mt>([](const spdlog::details::log_msg& msg) 
        { 
            bootMessages.emplace_back(String(msg.payload.data(), msg.payload.size()), msg);
        });

        _logger = CreatePtr<spdlog::logger>(programName, callbackSink);

        _logger->set_level(spdlog::level::trace);
        _logger->flush_on(spdlog::level::trace);

        spdlog::register_logger(_logger);

        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream timeStream;

#if defined (KMP_COMPILER_MSVC)
        struct tm buf{};
        localtime_s(&buf, &now);
        timeStream << std::put_time(&buf, "%F %T");
#else
        timeStream << std::put_time(localtime(&now), "%F %T");
#endif

        Log::Info("---------------------{}---------------------", timeStream.str());
    }
    //--------------------------------------------------------------------------

    void Log::Initialize(const LogSettings& settings, const String& programName)
    {
        spdlog::drop_all();
        spdlog::init_thread_pool(1024, 1);

        _logSettings = settings;

        if (_logSettings.enabled)
        {
            Vector<spdlog::sink_ptr> logSinks;

            if (_logSettings.outputConsole)
            {
                const auto stdoutSink = CreatePtr<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::automatic);
                stdoutSink->set_pattern("%^%T.%e %L \"%n\" | %v%$");
                logSinks.push_back(stdoutSink);
            }

            if (_logSettings.outputFile)
            {
                const auto fileSink = CreatePtr<spdlog::sinks::basic_file_sink_mt>(_logSettings.filename, _logSettings.truncate);
                fileSink->set_pattern("%T.%e %L \"%n\" | %v");
                logSinks.push_back(fileSink);
            }

            if (_logSettings.outputStringBuffer)
            {
                const auto stringBufferSink = CreatePtr<spdlog::sinks::ostream_sink_mt>(_stringStream);
                stringBufferSink->set_pattern("%T.%e %L \"%n\" | %v");
                logSinks.push_back(stringBufferSink);
            }

            const auto coreLevel = static_cast<spdlog::level::level_enum>(std::clamp(_logSettings.level, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));
            auto coreLevelFlush = static_cast<spdlog::level::level_enum>(std::clamp(_logSettings.levelFlush, SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_CRITICAL));
            if (coreLevelFlush < coreLevel)
            {
                coreLevelFlush = coreLevel;
            }

            std::for_each(logSinks.cbegin(), logSinks.cend(), [coreLevel](const spdlog::sink_ptr sink)
            {
                std::for_each(bootMessages.cbegin(), bootMessages.cend(), [&](const BootMessage& bootMsg)
                {
                    if (bootMsg.logMsg.level >= coreLevel)
                    {
                        sink->log(bootMsg.logMsg);
                    }
                });

                sink->flush();
            });

            _logger = CreatePtr<spdlog::async_logger>(programName, begin(logSinks), end(logSinks), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            _logger->set_level(coreLevel);
            _logger->flush_on(coreLevelFlush);
        }
        else
        {
            const auto nullSink = CreatePtr<spdlog::sinks::null_sink_mt>();
            _logger = CreatePtr<spdlog::async_logger>(programName, nullSink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        }

        spdlog::register_logger(_logger);

        bootMessages.clear();
    }
    //--------------------------------------------------------------------------

    void Log::Finalize()
    {
        _logger->flush();
        spdlog::drop_all();
        spdlog::shutdown();

        _logger = nullptr;
    }
    //--------------------------------------------------------------------------

    std::stringstream& Log::StringLogOutput()
    {
        return _stringStream;
    }
    //--------------------------------------------------------------------------
}
#endif