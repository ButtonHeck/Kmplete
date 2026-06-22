#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    //! Print current stacktrace to a console and dump it to a file
    //! with application name and the current timestamp
    KMP_API void DumpStacktrace();
}