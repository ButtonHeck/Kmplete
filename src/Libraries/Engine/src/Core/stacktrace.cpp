#include "Kmplete/Core/stacktrace.h"
#include "Kmplete/Application/application_context.h"
#include "Kmplete/Time/helper_functions.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/named_bool.h"

#include <cpptrace/cpptrace.hpp>


namespace Kmplete
{
    void DumpStacktrace()
    {
        const auto stacktrace = cpptrace::generate_trace();
        stacktrace.print();

        String dumpName = ApplicationContext::GetApplicationName();
        dumpName += " stacktrace ";
        dumpName += Time::GetCurrentTimeString();
        dumpName += ".txt";
        dumpName = Utils::RegexReplace(dumpName, ":", "_");
        const auto dumpFile = ApplicationContext::GetApplicationLogPath() / dumpName;

        Filesystem::WriteFile(dumpFile, stacktrace.to_string(), "append"_false);
    }
    //--------------------------------------------------------------------------
}