#include <string>

namespace FMNCV
{
    std::string getFMNCVVersion();
    unsigned int getFMNCVVersionMajor();
    unsigned int getFMNCVVersionMinor();
    unsigned int getFMNCVVersionPatch();
    unsigned int getFMNCVVersionTweak();
    
    std::string getFMNCVGitHash();
}