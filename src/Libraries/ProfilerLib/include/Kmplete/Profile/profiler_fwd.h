#pragma once

//! Helper macro definition to manually turn on/off profiling in the code (static builds only)
//! for Debug and Release builds
#define KMP_PROFILE_MANUAL_SET true

#if ((!defined KMP_BUILD_STATIC || KMP_PROFILE_MANUAL_SET) && !defined (KMP_CONFIG_TYPE_PRODUCTION)) || defined (KMP_CONFIG_TYPE_RELWITHDEBINFO)
#define KMP_PROFILE

#include "Kmplete/Base/pointers.h"

namespace Kmplete
{
    class ProfilerTimer;
}

#define KMP_PROFILE_CONSTRUCTOR_DECLARE() \
    private:\
    UPtr<ProfilerTimer> _constructorProfilerTimer;

#define KMP_PROFILE_CONSTRUCTOR_DECLARE_COPYABLE() \
    private:\
    Ptr<ProfilerTimer> _constructorProfilerTimer;

#else
#define KMP_PROFILE_CONSTRUCTOR_DECLARE()
#define KMP_PROFILE_CONSTRUCTOR_DECLARE_COPYABLE()
#endif


namespace Kmplete
{
    //! Enumeration of the profiling levels from the most important (0) to the least (4).
    //! There is no strict rule which level in which function should be used, but the common
    //! rule in this project is to use ProfileLevelAlways in constructors/destructors and functions
    //! like Initialize/Finalize, ProfileLevelImportant(Verbose) for somewhat "heavy" and
    //! important functions, and ProfileLevelMinor(Verbose) for any other functions
    enum ProfileLevel : unsigned int
    {
        ProfileLevelAlways = 0,
        ProfileLevelImportant = 1,
        ProfileLevelImportantVerbose = 2,
        ProfileLevelMinor = 3,
        ProfileLevelMinorVerbose = 4
    };
    //--------------------------------------------------------------------------
}