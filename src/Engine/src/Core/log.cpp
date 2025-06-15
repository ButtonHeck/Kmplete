#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)

#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/string_utils.h"

#include "spdlog/async.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/details/log_msg.h>

#include <chrono>
#include <algorithm>
#if defined (KMP_PLATFORM_LINUX) || defined (KMP_COMPILER_MINGW)
    #include <iomanip>
#endif

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "Log";
    constexpr static auto FilenameStr = "Filename";
    constexpr static auto EnabledStr = "Enabled";
    constexpr static auto TruncateStr = "Truncate";
    constexpr static auto OutputConsoleStr = "OutputConsole";
    constexpr static auto OutputFileStr = "OutputFile";
    constexpr static auto OutputStringBufferStr = "OutputStringBuffer";
    constexpr static auto LevelStr = "Level";
    constexpr static auto LevelFlushStr = "LevelFlush";

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

        static std::vector<BootMessage> bootMessages;
    }
    //--------------------------------------------------------------------------

    void Log::Boot()
    {
        bootMessages.reserve(64);

        const auto callbackSink = CreatePtr<spdlog::sinks::callback_sink_mt>([](const spdlog::details::log_msg& msg) 
        { 
            bootMessages.emplace_back(String(msg.payload.data(), msg.payload.size()), msg);
        });

        _logger = CreatePtr<spdlog::logger>("CORE", callbackSink);

        _logger->set_level(spdlog::level::trace);
        _logger->flush_on(spdlog::level::trace);

        spdlog::register_logger(_logger);

        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#if defined (KMP_COMPILER_MSVC)
        struct tm buf{};
        localtime_s(&buf, &now);
        Log::Info("---------------------{}---------------------", Utils::Concatenate(std::put_time(&buf, "%F %T")));
#else
        Log::Info("---------------------{}---------------------", Utils::Concatenate(std::put_time(localtime(&now), "%F %T")));
#endif
    }
    //--------------------------------------------------------------------------

    void Log::Initialize()
    {
        spdlog::drop_all();
        spdlog::init_thread_pool(1024, 1);

        if (_logSettings.enabled)
        {
            std::vector<spdlog::sink_ptr> logSinks;

            if (_logSettings.outputConsole)
            {
                const auto stdoutSink = CreatePtr<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::automatic);
                stdoutSink->set_pattern("%^[%T.%e] [%l] %n: %v%$");
                logSinks.push_back(stdoutSink);
            }

            if (_logSettings.outputFile)
            {
                const auto fileSink = CreatePtr<spdlog::sinks::basic_file_sink_mt>(_logSettings.filename, _logSettings.truncate);
                fileSink->set_pattern("[%T.%e] [%l] %n: %v");
                logSinks.push_back(fileSink);
            }

            if (_logSettings.outputStringBuffer)
            {
                const auto stringBufferSink = CreatePtr<spdlog::sinks::ostream_sink_mt>(_stringStream);
                stringBufferSink->set_pattern("[%T.%e] [%l] %n: %v");
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

            _logger = CreatePtr<spdlog::async_logger>("CORE", begin(logSinks), end(logSinks), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            _logger->set_level(coreLevel);
            _logger->flush_on(coreLevelFlush);
        }
        else
        {
            const auto nullSink = CreatePtr<spdlog::sinks::null_sink_mt>();
            _logger = CreatePtr<spdlog::async_logger>("CORE", nullSink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
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
    }
    //--------------------------------------------------------------------------

    std::string_view Log::StringLogOutput()
    {
        return _stringStream.view();
    }
    //--------------------------------------------------------------------------

    void Log::SaveSettings(Settings& settings)
    {
        settings.StartSaveObject(SettingsEntryName);
        settings.SaveString(FilenameStr, Utils::NarrowToUtf8(_logSettings.filename));
        settings.SaveBool(EnabledStr, _logSettings.enabled);
        settings.SaveBool(TruncateStr, _logSettings.truncate);
        settings.SaveBool(OutputConsoleStr, _logSettings.outputConsole);
        settings.SaveBool(OutputFileStr, _logSettings.outputFile);
        settings.SaveBool(OutputStringBufferStr, _logSettings.outputStringBuffer);
        settings.SaveInt(LevelStr, _logSettings.level);
        settings.SaveInt(LevelFlushStr, _logSettings.levelFlush);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void Log::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(SettingsEntryName);
        _logSettings.filename = Utils::Utf8ToNarrow(settings.GetString(FilenameStr, "Kmplete_log.txt"));
        _logSettings.enabled = settings.GetBool(EnabledStr, true);
        _logSettings.truncate = settings.GetBool(TruncateStr, false);
        _logSettings.outputConsole = settings.GetBool(OutputConsoleStr, true);
        _logSettings.outputFile = settings.GetBool(OutputFileStr, true);
        _logSettings.outputStringBuffer = settings.GetBool(OutputStringBufferStr, false);
        _logSettings.level = settings.GetInt(LevelStr, spdlog::level::trace);
        _logSettings.levelFlush = settings.GetInt(LevelFlushStr, spdlog::level::trace);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------
}
#endif