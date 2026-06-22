#include "Kmplete/Core/stacktrace.h"
#include "Kmplete/Application/application_context.h"
#include "Kmplete/Time/helper_functions.h"
#include "Kmplete/Utils/string_utils.h"

#include <cpptrace/cpptrace.hpp>

#include <fstream>


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

        std::ofstream outputFile(dumpFile);
        if (!outputFile.is_open())
        {
            return;
        }

        outputFile << stacktrace.to_string();
        outputFile.close();
    }
    //--------------------------------------------------------------------------
}