#pragma once


namespace Kmplete
{
    //! Kmplete project's version getters functions, definitions are
    //! generated automatically by CMake based on version variables and
    //! hash of the latest commit of the build

    const char* GetKmpleteVersion();
    unsigned int GetKmpleteVersionMajor();
    unsigned int GetKmpleteVersionMinor();
    unsigned int GetKmpleteVersionPatch();
    unsigned int GetKmpleteVersionTweak();
    
    const char* GetKmpleteGitHash();
}