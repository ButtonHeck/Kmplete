#pragma once

#include "kmplete_export.h"

#include <string>

namespace Kmplete
{
    class KMPLETE_API Application
    {
    public:
        Application();

        virtual ~Application() = default;

        virtual std::string GetApplicationName() const = 0;

        virtual bool Initialize(const std::string& configPath);
        virtual void Run() = 0;
    };
    //--------------------------------------------------------------------------

    extern Application* CreateApplication();
}