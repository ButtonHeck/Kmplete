#include "Kmplete/Application/window_application.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/log.h"

#include <stdexcept>

#ifdef CreateWindow
#pragma push_macro("CreateWindow")
#undef CreateWindow
#define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    constexpr static auto WindowAppSettingsEntryName = "WindowApplication";

    WindowApplication::WindowApplication(const std::filesystem::path& settingsFilePath, const std::string& defaultSettingsName)
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

    void WindowApplication::SaveSettings(const std::filesystem::path& path) const
    {
        SaveSettingsInternal();
        Application::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings(const std::filesystem::path& path)
    {
        Application::LoadSettings(path);
        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        _backend = WindowBackend::Create();
        KMP_ASSERT(_backend);

        LoadSettingsInternal();

        _mainWindow = _backend->CreateWindow("Main");
        if (!_mainWindow)
        {
            Log::CoreCritical("WindowApplication: creation of the main window failed");
            throw std::runtime_error("WindowApplication creation of the main window failed");
        }

        _mainWindow->SetIcon(KMP_DEFAULT_WINDOW_ICON_PATH);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        SaveSettingsInternal();

        _mainWindow.reset();
        _backend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettingsInternal() const
    {
        auto settings = _settingsManager->PutSettings(WindowAppSettingsEntryName);
        if (!settings)
        {
            Log::CoreWarn("WindowApplication: failed to create settings entry for saving");
            return;
        }

        _mainWindow->UpdateSettings();
        _backend->SaveSettings(settings);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettingsInternal()
    {
        const auto settings = _settingsManager->GetSettings(WindowAppSettingsEntryName);
        if (!settings)
        {
            Log::CoreWarn("WindowApplication: failed to get setting entry for loading");
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