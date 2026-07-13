#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    struct ApplicationParameters;


    //! Singleton collection of common application's properties. These properties may be
    //! viewed from any code after the base Application class was instantiated.
    //! @see Application
    class ApplicationContext
    {
        KMP_DISABLE_COPY_MOVE(ApplicationContext)

    public:
        KMP_NODISCARD KMP_API static const String& GetApplicationName() noexcept;
        KMP_NODISCARD KMP_API static const Filepath& GetApplicationPath() noexcept;
        KMP_NODISCARD KMP_API static const Filepath& GetApplicationDataPath() noexcept;
        KMP_NODISCARD KMP_API static const Filepath& GetApplicationLogPath() noexcept;

    private:
        friend class Application;
        static void _Initialize(const ApplicationParameters& parameters);

    private:
        static String _applicationName;
        static Filepath _applicationPath;
        static Filepath _dataPath;
        static Filepath _logPath;
    };
    //--------------------------------------------------------------------------
}