#include "Kmplete/Window/window_backend_glfw.h"
#include "Kmplete/Window/window_glfw.h"
#include "Kmplete/Window/window_cursor_glfw.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <GLFW/glfw3.h>

#include <stdexcept>


namespace Kmplete
{
    namespace
    {
        //! Utility function to fetch available monitor modes for a given monitor (if any)
        Vector<WindowBackend::MonitorVideoMode> GetVideoModes(Nullable<GLFWmonitor*> monitor)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

            Vector<WindowBackend::MonitorVideoMode> outputModes;

            if (!monitor)
            {
                return outputModes;
            }

            int modesCount = 0;
            auto modes = glfwGetVideoModes(monitor, &modesCount);
            if (modesCount && modes)
            {
                outputModes.reserve(modesCount);
                for (auto i = 0; i < modesCount; i++)
                {
                    auto mode = modes[i];
                    outputModes.emplace_back(mode.width, mode.height, mode.refreshRate);
                }
            }

            return outputModes;
        }
        //--------------------------------------------------------------------------
    }


    static constexpr auto MainWindowName = "Main";


    WindowBackendGlfw::WindowBackendGlfw()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _Initialize();
    }
    //--------------------------------------------------------------------------

    WindowBackendGlfw::~WindowBackendGlfw()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _Finalize();
    }
    //--------------------------------------------------------------------------

    Window& WindowBackendGlfw::CreateMainWindow()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (_mainWindow)
        {
            KMP_LOG_WARN("main window already created!");
        }
        else
        {
            if (!_mainWindowSettings)
            {
                KMP_LOG_WARN("main window settings were not found, provide default");
                _mainWindowSettings = CreateUPtr<Window::WindowSettings>(MainWindowName);
            }

            _mainWindow = CreateUPtr<WindowGlfw>(*_mainWindowSettings, _graphicsBackendType);
        }

        return *_mainWindow;
    }
    //--------------------------------------------------------------------------

    Window& WindowBackendGlfw::GetMainWindow()
    {
        KMP_ASSERT(_mainWindow);
        return *_mainWindow;
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_Initialize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        int glfwInitExitCode = GLFW_FALSE;
        {
            KMP_PROFILE_SCOPE("GLFW initialization", ProfileLevelAlways);
            glfwInitExitCode = glfwInit();
        }

        if (glfwInitExitCode != GLFW_TRUE)
        {
            KMP_MB_UNUSED const char* description;
            KMP_MB_UNUSED const auto errorCode = glfwGetError(&description);
            KMP_LOG_CRITICAL("initialization error: code '{}', description '{}'", errorCode, description ? description : "");
            throw std::runtime_error("WindowBackendGlfw initialization failed");
        }

        _InitializeCallbacks();

        _mainWindowSettings = CreateUPtr<Window::WindowSettings>(MainWindowName);
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_InitializeCallbacks() const
    {
        _InitializeErrorCallback();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_InitializeErrorCallback() const
    {
        glfwSetErrorCallback([](KMP_MB_UNUSED int code, KMP_MB_UNUSED const char* description) {
            KMP_LOG_ERROR("GLFW internal error '{}': {}", code, description);
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _mainWindow.reset();
        _mainWindowSettings.reset();

        _auxWindows.clear();

        if (glfwGetCurrentContext())
        {
            KMP_LOG_WARN("some window's context is still current");
        }

        _auxWindowsSettings.clear();
        _cursors.clear();

        {
            KMP_PROFILE_SCOPE("GLFW termination", ProfileLevelAlways);
            glfwTerminate();
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::CreateAuxWindow(const String& windowName)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (!_IsAuxWindowNameAcceptable(windowName))
        {
            return nullptr;
        }

        try
        {
            if (_auxWindowsSettings.contains(windowName))
            {
                KMP_LOG_INFO("creating window '{}' with previously loaded settings", windowName);
            }
            else
            {
                KMP_LOG_INFO("creating window '{}' with default settings", windowName);
                _auxWindowsSettings.emplace(windowName, CreateUPtr<Window::WindowSettings>(windowName));
            }

            _auxWindows.emplace(windowName, CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName], _graphicsBackendType));
            return GetAuxWindow(windowName);
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_ERROR("error creating window '{}', message: '{}'", windowName, e.what());
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::CreateAuxWindow(const Window::WindowSettings& windowSettings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        const auto& windowName = windowSettings.name;
        if (!_IsAuxWindowNameAcceptable(windowName))
        {
            return nullptr;
        }

        try
        {
            if (_auxWindowsSettings.contains(windowName))
            {
                KMP_LOG_WARN("creating window '{}' with previously loaded settings (provided settings will be discarded)", windowName);
            }
            else
            {
                KMP_LOG_INFO("creating window '{}' with provided settings", windowName);
                _auxWindowsSettings.emplace(windowName, CreateUPtr<Window::WindowSettings>(windowSettings));
            }

            _auxWindows.emplace(windowName, CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName], _graphicsBackendType));
            return GetAuxWindow(windowName);
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_ERROR("error creating window '{}', message: '{}'", windowName, e.what());
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::GetAuxWindow(const String& windowName) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (_auxWindows.contains(windowName))
        {
            return _auxWindows.at(windowName).get();
        }

        KMP_LOG_WARN("cannot find aux window '{}'", windowName);
        return nullptr;
    }
    //--------------------------------------------------------------------------

    bool WindowBackendGlfw::DestroyAuxWindow(const String& windowName)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_auxWindows.contains(windowName))
        {
            _auxWindows.erase(windowName);
            return true;
        }
        
        KMP_LOG_WARN("cannot find aux window '{}'", windowName);
        return false;
    }
    //--------------------------------------------------------------------------

    int WindowBackendGlfw::GetMonitorCount() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        int count = 0;
        glfwGetMonitors(&count);
        return count;
    }
    //--------------------------------------------------------------------------

    StringVector WindowBackendGlfw::GetMonitorNames() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        int count = 0;
        const auto monitors = glfwGetMonitors(&count);

        StringVector names;
        if (count && monitors)
        {
            names.reserve(count);
            for (auto i = 0; i < count; i++)
            {
                const auto monitor = monitors[i];
                if (monitor)
                {
                    names.emplace_back(glfwGetMonitorName(monitor));
                }
            }
        }

        return names;
    }
    //--------------------------------------------------------------------------

    Vector<WindowBackend::MonitorVideoMode> WindowBackendGlfw::GetPrimaryMonitorVideoModes() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const auto monitor = glfwGetPrimaryMonitor();
        return GetVideoModes(monitor);
    }
    //--------------------------------------------------------------------------

    Vector<WindowBackend::MonitorVideoMode> WindowBackendGlfw::GetMonitorVideoModes(unsigned int index) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        if (index == 0)
        {
            return GetPrimaryMonitorVideoModes();
        }

        int count = 0;
        const auto monitors = glfwGetMonitors(&count);
        if (monitors && index < static_cast<unsigned int>(count))
        {
            const auto monitor = monitors[index];
            return GetVideoModes(monitor);
        }

        return Vector<WindowBackend::MonitorVideoMode>();
    }
    //--------------------------------------------------------------------------

    float WindowBackendGlfw::GetPrimaryMonitorDPIScale() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const auto monitor = glfwGetPrimaryMonitor();
        float scale = 1.0f;
        glfwGetMonitorContentScale(monitor, &scale, &scale);
        return scale;
    }
    //--------------------------------------------------------------------------

    OptionalRef<const WindowCursor> WindowBackendGlfw::AddCursor(const String& name, const Filepath& filepath, const Math::Point2I& hotspot /*= Math::Point2I()*/)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (_cursors.contains(name))
        {
            KMP_LOG_WARN("cursor named '{}' already added", name);
            return std::cref(*_cursors.at(name).get());
        }

        try
        {
            _cursors.emplace(name, CreateUPtr<WindowCursorGlfw>(filepath, hotspot));
        }
        catch (KMP_MB_UNUSED const std::runtime_error& error)
        {
            KMP_LOG_ERROR("failed to add cursor '{}' from '{}' - {}", name, filepath, error.what());
            return std::nullopt;
        }

        return std::cref(*_cursors.at(name).get());
    }
    //--------------------------------------------------------------------------

    OptionalRef<const WindowCursor> WindowBackendGlfw::GetCursor(const String& name) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        if (!_cursors.contains(name))
        {
            KMP_LOG_WARN("cannot find cursor named '{}'", name);
            return std::nullopt;
        }

        return std::cref(*_cursors.at(name).get());
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartSaveObject(SettingsEntryName);

        _SaveMainWindowSettings(settings);
        _SaveAuxWindowsSettings(settings);

        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_SaveMainWindowSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartSaveObject(MainWindowStr);
        _mainWindowSettings->SaveSettings(settings);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_SaveAuxWindowsSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartSaveArray(AuxWindowsStr);
        int index = 0;
        for (const auto& windowEntry : _auxWindowsSettings)
        {
            KMP_ASSERT(windowEntry.second);
            const auto& windowSettings = *windowEntry.second;

            settings.StartSaveObject(index);
            windowSettings.SaveSettings(settings);
            settings.EndSaveObject();

            ++index;
        }

        settings.EndSaveArray();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartLoadObject(SettingsEntryName);

        _LoadMainWindowSettings(settings);
        _LoadAuxWindowsSettings(settings);

        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_LoadMainWindowSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        KMP_ASSERT(_mainWindowSettings);

        if (!settings.StartLoadObject(MainWindowStr))
        {
            KMP_LOG_WARN("main window settings were not found");
        }
        else
        {
            _mainWindowSettings->LoadSettings(settings);
        }

        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::_LoadAuxWindowsSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        const auto windowsCount = settings.StartLoadArray(AuxWindowsStr);
        for (auto i = 0; i < windowsCount; i++)
        {
            settings.StartLoadObject(i);
            Window::WindowSettings windowSettings;
            windowSettings.LoadSettings(settings);
            settings.EndLoadObject();

            if (windowSettings.name.empty())
            {
                KMP_LOG_ERROR("loading settings for unnamed window is prohibited, current object would be skipped, remove it from settings");
            }
            else
            {
                _auxWindowsSettings.emplace(windowSettings.name, CreateUPtr<Window::WindowSettings>(windowSettings));
            }
        }

        settings.EndLoadArray();
    }
    //--------------------------------------------------------------------------

    bool WindowBackendGlfw::_IsAuxWindowNameAcceptable(const String& name) const noexcept
    {
        if (name.empty())
        {
            KMP_LOG_ERROR("creation of unnamed windows is prohibited");
            return false;
        }
        else if (name == MainWindowName)
        {
            KMP_LOG_ERROR("cannot create two \"Main\" windows");
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}
