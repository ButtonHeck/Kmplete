#include "Kmplete/Application/application_context.h"
#include "Kmplete/Application/application.h"
#include "Kmplete/Filesystem/filesystem.h"


namespace Kmplete
{
    /*static*/ String ApplicationContext::_applicationName;
    /*static*/ Filepath ApplicationContext::_applicationPath;
    /*static*/ Filepath ApplicationContext::_dataPath;
    /*static*/ Filepath ApplicationContext::_logPath;


    const String& ApplicationContext::GetApplicationName() noexcept
    {
        return _applicationName;
    }
    //--------------------------------------------------------------------------

    const Filepath& ApplicationContext::GetApplicationPath() noexcept
    {
        return _applicationPath;
    }
    //--------------------------------------------------------------------------

    const Filepath& ApplicationContext::GetApplicationDataPath() noexcept
    {
        return _dataPath;
    }
    //--------------------------------------------------------------------------

    const Filepath& ApplicationContext::GetApplicationLogPath() noexcept
    {
        return _logPath;
    }
    //--------------------------------------------------------------------------

    void ApplicationContext::_Initialize(const ApplicationParameters& parameters)
    {
        _applicationName = parameters.applicationName;
        _applicationPath = Filesystem::GetCurrentFilepath();
        _dataPath = _applicationPath / "Data";
        _logPath = _applicationPath / "Logs";
    }
    //--------------------------------------------------------------------------
}