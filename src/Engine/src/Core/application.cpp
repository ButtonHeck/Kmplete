#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"

namespace Kmplete
{
    Application::Application()
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize(const std::string& configPath)
    {
        Filesystem::Initialize();

        return true;
    }
    //--------------------------------------------------------------------------
}