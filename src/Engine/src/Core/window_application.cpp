#include "Kmplete/Core/window_application.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/log.h"

#ifdef CreateWindow
#pragma push_macro("CreateWindow")
#undef CreateWindow
#define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    constexpr static auto WindowAppSettingsEntryName = "WindowApplication";

    WindowApplication::WindowApplication(const std::string& settingsFilePath, const std::string& defaultSettingsName)
        : Application(settingsFilePath, defaultSettingsName)
        , _backend(nullptr)
        , _mainWindow(nullptr)
    {
        Initialize();

        Log::CoreTrace("WindowApplication: created");
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        Finalize();

        Log::CoreTrace("WindowApplication: destroyed");
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        _backend = WindowBackend::Create();
        KMP_ASSERT(_backend);

        LoadSettings();

        _mainWindow = _backend->CreateWindow("Main");
        if (!_mainWindow)
        {
            Log::CoreCritical("WindowApplication: creation of the main window failed");
            throw std::exception("WindowApplication creation of the main window failed");
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        _mainWindow->UpdateSettings();
        SaveSettings();

        _mainWindow.reset();
        _backend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        auto settings = _settingsManager->PutSettings(WindowAppSettingsEntryName);
        if (!settings)
        {
            return;
        }

        _backend->SaveSettings(settings);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings() const
    {
        const auto settings = _settingsManager->GetSettings(WindowAppSettingsEntryName);
        if (!settings)
        {
            return;
        }

        _backend->LoadSettings(settings);
    }
    //--------------------------------------------------------------------------
}

#ifdef KMP_UNDEF_CreateWindow
#pragma pop_macro("CreateWindow")
#undef KMP_UNDEF_CreateWindow
#endif