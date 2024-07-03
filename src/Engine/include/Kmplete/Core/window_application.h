#pragma once

#include "kmplete_export.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/application.h"

namespace Kmplete
{
    class KMPLETE_API WindowApplication : public Application
    {
    public:
        WindowApplication();

        bool Initialize(const std::string& configPath) override;
    };
    //--------------------------------------------------------------------------
}