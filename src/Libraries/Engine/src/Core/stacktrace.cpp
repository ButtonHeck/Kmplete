#include "Kmplete/Core/stacktrace.h"
#include "Kmplete/Application/application_context.h"
#include "Kmplete/Time/helper_functions.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Log/log.h"

#include <cpptrace/cpptrace.hpp>


namespace Kmplete
{
    void DumpStacktrace()
    {
        const auto stacktrace = cpptrace::generate_trace();
        stacktrace.print();

        auto dumpName = Utils::Concatenate(ApplicationContext::GetApplicationName(), " stacktrace ", Time::GetCurrentTimeString(), ".txt");
        dumpName = Utils::RegexReplace(dumpName, ":", "_");
        const auto dumpFile = ApplicationContext::GetApplicationLogPath() / dumpName;

        if (Filesystem::WriteFile(dumpFile, stacktrace.to_string(), "append"_false))
        {
            KMP_LOG_ERROR_FN("stacktrace written to a file '{}'", dumpFile);
        }
    }
    //--------------------------------------------------------------------------
}