#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    struct ApplicationParameters;


    //TODO: comments
    class ApplicationContext
    {
        KMP_DISABLE_COPY_MOVE(ApplicationContext)

    public:
        KMP_NODISCARD KMP_API static const String& GetApplicationName() noexcept;
        KMP_NODISCARD KMP_API static const Filepath& GetApplicationPath() noexcept;
        KMP_NODISCARD KMP_API static const Filepath& GetApplicationDataPath() noexcept;

    private:
        friend class Application;
        static void _Initialize(const ApplicationParameters& parameters);

    private:
        static String _applicationName;
        static Filepath _applicationPath;
        static Filepath _dataPath;
    };
    //--------------------------------------------------------------------------
}