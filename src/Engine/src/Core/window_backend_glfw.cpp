#include "Kmplete/Core/window_backend_glfw.h"
#include "Kmplete/Core/window_glfw.h"
#include "Kmplete/Core/window_cursor_glfw.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

#if defined (CreateWindow)
    #pragma push_macro("CreateWindow")
    #undef CreateWindow
    #define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    namespace
    {
        Vector<WindowBackend::MonitorVideoMode> GetVideoModes(Nullable<GLFWmonitor*> monitor)
        {
            KMP_PROFILE_FUNCTION();

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

    constexpr static auto MainWindowName = "Main";


    WindowBackendGlfw::WindowBackendGlfw()
    {
        KMP_PROFILE_FUNCTION();

        Initialize();
    }
    //--------------------------------------------------------------------------

    WindowBackendGlfw::~WindowBackendGlfw()
    {
        KMP_PROFILE_FUNCTION();

        Finalize();
    }
    //--------------------------------------------------------------------------

    Window& WindowBackendGlfw::CreateMainWindow()
    {
        KMP_PROFILE_FUNCTION();

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

            _mainWindow = CreateUPtr<WindowGlfw>(*_mainWindowSettings);
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

    void WindowBackendGlfw::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        int glfwInitExitCode = GLFW_FALSE;
        {
            KMP_PROFILE_SCOPE("GLFW initialization");
            glfwInitExitCode = glfwInit();
        }

        if (glfwInitExitCode != GLFW_TRUE)
        {
            KMP_MB_UNUSED const char* description;
            KMP_MB_UNUSED const auto errorCode = glfwGetError(&description);
            KMP_LOG_CRITICAL("initialization error: code '{}', description '{}'", errorCode, description ? description : "");
            throw std::runtime_error("WindowBackendGlfw initialization failed");
        }

        _mainWindowSettings = CreateUPtr<Window::WindowSettings>(MainWindowName);
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::Finalize()
    {
        KMP_PROFILE_FUNCTION();

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
            KMP_PROFILE_SCOPE("GLFW termination");
            glfwTerminate();
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::CreateAuxWindow(const String& windowName)
    {
        KMP_PROFILE_FUNCTION();

        if (windowName.empty())
        {
            KMP_LOG_ERROR("creation of unnamed windows is prohibited");
            return nullptr;
        }

        if (windowName == MainWindowName)
        {
            KMP_LOG_ERROR("cannot create two \"Main\" windows");
            return nullptr;
        }

        if (_auxWindowsSettings.contains(windowName))
        {
            KMP_LOG_INFO("creating window '{}' with previously loaded settings", windowName);
            try
            {
                _auxWindows[windowName] = CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName]);
                return GetAuxWindow(windowName);
            }
            catch (KMP_MB_UNUSED const std::exception& e)
            {
                KMP_LOG_ERROR("error creating window '{}', message: '{}'", windowName, e.what());
                return nullptr;
            }
        }
        else
        {
            KMP_LOG_INFO("creating window '{}' with default settings", windowName);
            _auxWindowsSettings[windowName] = CreateUPtr<Window::WindowSettings>(windowName);

            try
            {
                _auxWindows[windowName] = CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName]);
                return GetAuxWindow(windowName);
            }
            catch (KMP_MB_UNUSED const std::exception& e)
            {
                KMP_LOG_ERROR("error creating window '{}', message: '{}'", windowName, e.what());
                return nullptr;
            }
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::CreateAuxWindow(Window::WindowSettings& windowSettings)
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            if (windowSettings.name.empty() || windowSettings.name == MainWindowName)
            {
                throw std::exception();
            }

            const auto& windowName = windowSettings.name;
            if (_auxWindowsSettings.contains(windowName))
            {
                KMP_LOG_WARN("window '{}' will be created, but settings already contains this name and will be overriden", windowName);
            }
            else
            {
                KMP_LOG_INFO("window '{}' will be created with provided settings", windowName);
                _auxWindowsSettings.insert({ windowName, CreateUPtr<Window::WindowSettings>(windowSettings) });
            }

            _auxWindows[windowName] = CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName]);
            return GetAuxWindow(windowName);
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_ERROR("error creating window '{}', message: '{}'", windowSettings.name, e.what());
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::GetAuxWindow(const String& windowName) const
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

        int count = 0;
        glfwGetMonitors(&count);
        return count;
    }
    //--------------------------------------------------------------------------

    StringVector WindowBackendGlfw::GetMonitorNames() const
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

        const auto monitor = glfwGetPrimaryMonitor();
        return GetVideoModes(monitor);
    }
    //--------------------------------------------------------------------------

    Vector<WindowBackend::MonitorVideoMode> WindowBackendGlfw::GetMonitorVideoModes(int index) const
    {
        KMP_PROFILE_FUNCTION();

        if (index == 0)
        {
            return GetPrimaryMonitorVideoModes();
        }

        int count = 0;
        const auto monitors = glfwGetMonitors(&count);
        if (monitors && index < count)
        {
            const auto monitor = monitors[index];
            return GetVideoModes(monitor);
        }

        return Vector<WindowBackend::MonitorVideoMode>();
    }
    //--------------------------------------------------------------------------

    float WindowBackendGlfw::GetPrimaryMonitorDPIScale() const
    {
        KMP_PROFILE_FUNCTION();

        const auto monitor = glfwGetPrimaryMonitor();
        float scale = 1.0f;
        glfwGetMonitorContentScale(monitor, &scale, &scale);
        return scale;
    }
    //--------------------------------------------------------------------------

    Nullable<WindowCursor*> WindowBackendGlfw::AddCursor(const String& name, const Filepath& filepath, int hotspotX /*= 0*/, int hotspotY /*= 0*/)
    {
        if (_cursors.contains(name))
        {
            KMP_LOG_WARN("cursor named '{}' already added", name);
            return _cursors.at(name).get();
        }

        try
        {
            _cursors.emplace(name, CreateUPtr<WindowCursorGlfw>(filepath, hotspotX, hotspotY));
        }
        catch (const std::runtime_error& error)
        {
            KMP_LOG_ERROR("failed to add cursor '{}' from '{}' ({})", name, filepath, error.what());
            return nullptr;
        }

        return _cursors.at(name).get();
    }
    //--------------------------------------------------------------------------

    Nullable<WindowCursor*> WindowBackendGlfw::GetCursor(const String& name) const
    {
        if (!_cursors.contains(name))
        {
            KMP_LOG_WARN("cannot find cursor named '{}'", name);
            return nullptr;
        }

        return _cursors.at(name).get();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveSettings(Settings& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveObject(SettingsEntryName);

        SaveMainWindowSettings(settings);
        SaveAuxWindowsSettings(settings);

        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveMainWindowSettings(Settings& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveObject(MainWindowStr);

        settings.SaveString(Window::NameStr, _mainWindowSettings->name);
        settings.SaveInt(Window::WidthStr, _mainWindowSettings->width);
        settings.SaveInt(Window::HeightStr, _mainWindowSettings->height);
        settings.SaveInt(Window::WindowedWidthStr, _mainWindowSettings->windowedWidth);
        settings.SaveInt(Window::WindowedHeightStr, _mainWindowSettings->windowedHeight);
        settings.SaveString(Window::ScreenModeStr, Window::ScreenModeToString(_mainWindowSettings->screenMode));
        settings.SaveBool(Window::VSyncStr, _mainWindowSettings->vSync);
        settings.SaveBool(Window::UpdateContinuouslyStr, _mainWindowSettings->updateContinuously);
        settings.SaveBool(Window::ResizableStr, _mainWindowSettings->resizable);
        settings.SaveBool(Window::DecoratedStr, _mainWindowSettings->decorated);

        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveAuxWindowsSettings(Settings& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveArray(AuxWindowsStr);
        int index = 0;
        for (const auto& windowEntry : _auxWindowsSettings)
        {
            KMP_ASSERT(windowEntry.second);
            const auto& windowSettings = *windowEntry.second;

            settings.StartSaveObject(index);

            settings.SaveString(Window::NameStr, windowSettings.name);
            settings.SaveInt(Window::WidthStr, windowSettings.width);
            settings.SaveInt(Window::HeightStr, windowSettings.height);
            settings.SaveInt(Window::WindowedWidthStr, windowSettings.windowedWidth);
            settings.SaveInt(Window::WindowedHeightStr, windowSettings.windowedHeight);
            settings.SaveString(Window::ScreenModeStr, Window::ScreenModeToString(windowSettings.screenMode));
            settings.SaveBool(Window::VSyncStr, windowSettings.vSync);
            settings.SaveBool(Window::UpdateContinuouslyStr, windowSettings.updateContinuously);
            settings.SaveBool(Window::ResizableStr, windowSettings.resizable);
            settings.SaveBool(Window::DecoratedStr, windowSettings.decorated);

            settings.EndSaveObject();
            ++index;
        }

        settings.EndSaveArray();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadSettings(Settings& settings)
    {
        KMP_PROFILE_FUNCTION();

        settings.StartLoadObject(SettingsEntryName);

        LoadMainWindowSettings(settings);
        LoadAuxWindowsSettings(settings);

        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadMainWindowSettings(Settings& settings)
    {
        KMP_PROFILE_FUNCTION();

        KMP_ASSERT(_mainWindowSettings);

        if (!settings.StartLoadObject(MainWindowStr))
        {
            KMP_LOG_WARN("main window settings were not found");
        }
        else
        {
            _mainWindowSettings->width = settings.GetInt(Window::WidthStr, Window::DefaultWidth);
            _mainWindowSettings->height = settings.GetInt(Window::HeightStr, Window::DefaultHeight);
            _mainWindowSettings->windowedWidth = settings.GetInt(Window::WindowedWidthStr, Window::DefaultWidth);
            _mainWindowSettings->windowedHeight = settings.GetInt(Window::WindowedHeightStr, Window::DefaultHeight);
            _mainWindowSettings->screenMode = Window::StringToScreenMode(settings.GetString(Window::ScreenModeStr, Window::WindowedModeStr));
            _mainWindowSettings->vSync = settings.GetBool(Window::VSyncStr, true);
            _mainWindowSettings->updateContinuously = settings.GetBool(Window::UpdateContinuouslyStr, true);
            _mainWindowSettings->resizable = settings.GetBool(Window::ResizableStr, true);
            _mainWindowSettings->decorated = settings.GetBool(Window::DecoratedStr, true);
        }

        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadAuxWindowsSettings(Settings& settings)
    {
        KMP_PROFILE_FUNCTION();

        const auto windowsCount = settings.StartLoadArray(AuxWindowsStr);
        for (auto i = 0; i < windowsCount; i++)
        {
            settings.StartLoadObject(i);

            const auto windowName = settings.GetString(Window::NameStr, "");
            if (windowName.empty())
            {
                KMP_LOG_ERROR("loading settings for unnamed window is prohibited, current object would be skipped, remove it from settings");
            }
            else
            {
                auto windowSettings = CreateUPtr<Window::WindowSettings>(windowName);
                windowSettings->width = settings.GetInt(Window::WidthStr, Window::DefaultWidth);
                windowSettings->height = settings.GetInt(Window::HeightStr, Window::DefaultHeight);
                windowSettings->windowedWidth = settings.GetInt(Window::WindowedWidthStr, Window::DefaultWidth);
                windowSettings->windowedHeight = settings.GetInt(Window::WindowedHeightStr, Window::DefaultHeight);
                windowSettings->screenMode = Window::StringToScreenMode(settings.GetString(Window::ScreenModeStr, Window::WindowedModeStr));
                windowSettings->vSync = settings.GetBool(Window::VSyncStr, true);
                windowSettings->updateContinuously = settings.GetBool(Window::UpdateContinuouslyStr, true);
                windowSettings->resizable = settings.GetBool(Window::ResizableStr, true);
                windowSettings->decorated = settings.GetBool(Window::DecoratedStr, true);
                _auxWindowsSettings.insert({ windowName, std::move(windowSettings) });
            }

            settings.EndLoadObject();
        }

        settings.EndLoadArray();
    }
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_CreateWindow)
    #pragma pop_macro("CreateWindow")
    #undef KMP_UNDEF_CreateWindow
#endif
