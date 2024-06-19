#include <string>

namespace Kmplete
{
    std::string getKmpleteVersion();
    unsigned int getKmpleteVersionMajor();
    unsigned int getKmpleteVersionMinor();
    unsigned int getKmpleteVersionPatch();
    unsigned int getKmpleteVersionTweak();
    
    std::string getKmpleteGitHash();
}