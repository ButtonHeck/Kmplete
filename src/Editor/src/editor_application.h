#pragma once

#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    class EditorApplication : public WindowApplication
    {
    public:
        EditorApplication();
        ~EditorApplication();

        std::string GetApplicationName() const override;

        bool Initialize(const std::string& configPath) override;
        void Run() override;
    };
    //--------------------------------------------------------------------------
}