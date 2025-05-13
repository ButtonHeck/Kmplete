#pragma once

namespace Kmplete
{
    const char* GetKmpleteVersion();
    unsigned int GetKmpleteVersionMajor();
    unsigned int GetKmpleteVersionMinor();
    unsigned int GetKmpleteVersionPatch();
    unsigned int GetKmpleteVersionTweak();
    
    const char* GetKmpleteGitHash();
}