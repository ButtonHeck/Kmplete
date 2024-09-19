#include <string>

namespace Kmplete
{
    std::string GetKmpleteVersion();
    unsigned int GetKmpleteVersionMajor();
    unsigned int GetKmpleteVersionMinor();
    unsigned int GetKmpleteVersionPatch();
    unsigned int GetKmpleteVersionTweak();
    
    std::string GetKmpleteGitHash();
}