#include "Kmplete/Core/window_backend_glfw.h"
#include "Kmplete/Core/window_glfw.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"

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
        std::vector<WindowBackend::MonitorVideoMode> GetVideoModes(GLFWmonitor* monitor)
        {
            std::vector<WindowBackend::MonitorVideoMode> outputModes;

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
        Initialize();
    }
    //--------------------------------------------------------------------------

    WindowBackendGlfw::~WindowBackendGlfw()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    Window& WindowBackendGlfw::CreateMainWindow()
    {
        if (_mainWindow)
        {
            KMP_LOG_CORE_WARN("WindowBackendGlfw: main window already created!");
        }
        else
        {
            if (!_mainWindowSettings)
            {
                KMP_LOG_CORE_WARN("WindowBackendGlfw: main window settings were not found, provide default");
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
        if (!glfwInit())
        {
            KMP_MB_UNUSED const char* description;
            KMP_MB_UNUSED const auto errorCode = glfwGetError(&description);
            KMP_LOG_CORE_CRITICAL("WindowBackendGlfw: initialization error: code '{}', description '{}'", errorCode, description ? description : "");
            throw std::runtime_error("WindowBackendGlfw initialization failed");
        }

        _mainWindowSettings = CreateUPtr<Window::WindowSettings>(MainWindowName);
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::Finalize()
    {
        _mainWindow.reset();
        _mainWindowSettings.reset();

        _auxWindows.clear();

        if (glfwGetCurrentContext())
        {
            KMP_LOG_CORE_WARN("WindowBackendGlfw: some window's context is still current");
        }

        _auxWindowsSettings.clear();
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::CreateAuxWindow(const String& windowName)
    {
        if (windowName.empty())
        {
            KMP_LOG_CORE_ERROR("WindowBackendGlfw: creation of unnamed windows is prohibited");
            return nullptr;
        }

        if (windowName == MainWindowName)
        {
            KMP_LOG_CORE_ERROR("WindowBackendGlfw: cannot create two \"Main\" windows");
            return nullptr;
        }

        if (_auxWindowsSettings.contains(windowName))
        {
            KMP_LOG_CORE_INFO("WindowBackendGlfw: creating window '{}' with previously loaded settings", windowName);
            try
            {
                _auxWindows[windowName] = CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName]);
                return GetAuxWindow(windowName);
            }
            catch (KMP_MB_UNUSED const std::exception& e)
            {
                KMP_LOG_CORE_ERROR("WindowBackendGlfw: error creating window '{}', message: '{}'", windowName, e.what());
                return nullptr;
            }
        }
        else
        {
            KMP_LOG_CORE_INFO("WindowBackendGlfw: creating window '{}' with default settings", windowName);
            _auxWindowsSettings[windowName] = CreateUPtr<Window::WindowSettings>(windowName);

            try
            {
                _auxWindows[windowName] = CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName]);
                return GetAuxWindow(windowName);
            }
            catch (KMP_MB_UNUSED const std::exception& e)
            {
                KMP_LOG_CORE_ERROR("WindowBackendGlfw: error creating window '{}', message: '{}'", windowName, e.what());
                return nullptr;
            }
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::CreateAuxWindow(Window::WindowSettings& windowSettings)
    {
        try
        {
            if (windowSettings.name.empty() || windowSettings.name == MainWindowName)
            {
                throw std::exception();
            }

            const auto& windowName = windowSettings.name;
            if (_auxWindowsSettings.contains(windowName))
            {
                KMP_LOG_CORE_WARN("WindowBackendGlfw: window '{}' will be created, but settings already contains this name and will be overriden", windowName);
            }
            else
            {
                KMP_LOG_CORE_INFO("WindowBackendGlfw: window '{}' will be created with provided settings", windowName);
                _auxWindowsSettings.insert({ windowName, CreateUPtr<Window::WindowSettings>(windowSettings) });
            }

            _auxWindows[windowName] = CreateUPtr<WindowGlfw>(*_auxWindowsSettings[windowName]);
            return GetAuxWindow(windowName);
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_CORE_ERROR("WindowBackendGlfw: error creating window '{}', message: '{}'", windowSettings.name, e.what());
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    Nullable<Window*> WindowBackendGlfw::GetAuxWindow(const String& windowName) const
    {
        if (_auxWindows.contains(windowName))
        {
            return _auxWindows.at(windowName).get();
        }

        KMP_LOG_CORE_WARN("WindowBackendGlfw: cannot find aux window '{}'", windowName);
        return nullptr;
    }
    //--------------------------------------------------------------------------

    bool WindowBackendGlfw::DestroyAuxWindow(const String& windowName)
    {
        if (_auxWindows.contains(windowName))
        {
            _auxWindows.erase(windowName);
            return true;
        }
        
        KMP_LOG_CORE_WARN("WindowBackendGlfw: cannot find aux window '{}'", windowName);
        return false;
    }
    //--------------------------------------------------------------------------

    int WindowBackendGlfw::GetMonitorCount() const
    {
        int count = 0;
        glfwGetMonitors(&count);
        return count;
    }
    //--------------------------------------------------------------------------

    StringVector WindowBackendGlfw::GetMonitorNames() const
    {
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

    std::vector<WindowBackend::MonitorVideoMode> WindowBackendGlfw::GetPrimaryMonitorVideoModes() const
    {
        const auto monitor = glfwGetPrimaryMonitor();
        return GetVideoModes(monitor);
    }
    //--------------------------------------------------------------------------

    std::vector<WindowBackend::MonitorVideoMode> WindowBackendGlfw::GetMonitorVideoModes(int index) const
    {
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

        return std::vector<WindowBackend::MonitorVideoMode>();
    }
    //--------------------------------------------------------------------------

    float WindowBackendGlfw::GetPrimaryMonitorDPIScale() const
    {
        const auto monitor = glfwGetPrimaryMonitor();
        float scale = 1.0f;
        glfwGetMonitorContentScale(monitor, &scale, &scale);
        return scale;
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(SettingsEntryName);

        SaveMainWindowSettings(settings);
        SaveAuxWindowsSettings(settings);

        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveMainWindowSettings(Settings& settings) const
    {
        settings.StartSaveObject(MainWindowStr);

        settings.SaveString(Window::NameStr, _mainWindowSettings->name);
        settings.SaveInt(Window::WidthStr, _mainWindowSettings->width);
        settings.SaveInt(Window::HeightStr, _mainWindowSettings->height);
        settings.SaveInt(Window::WindowedWidthStr, _mainWindowSettings->windowedWidth);
        settings.SaveInt(Window::WindowedHeightStr, _mainWindowSettings->windowedHeight);
        settings.SaveString(Window::ScreenModeStr, Window::ModeToString(_mainWindowSettings->screenMode));
        settings.SaveBool(Window::VSyncStr, _mainWindowSettings->vSync);
        settings.SaveBool(Window::UpdateContinuouslyStr, _mainWindowSettings->updateContinuously);
        settings.SaveBool(Window::ResizableStr, _mainWindowSettings->resizable);
        settings.SaveBool(Window::DecoratedStr, _mainWindowSettings->decorated);

        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::SaveAuxWindowsSettings(Settings& settings) const
    {
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
            settings.SaveString(Window::ScreenModeStr, Window::ModeToString(windowSettings.screenMode));
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
        settings.StartLoadObject(SettingsEntryName);

        LoadMainWindowSettings(settings);
        LoadAuxWindowsSettings(settings);

        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadMainWindowSettings(Settings& settings)
    {
        KMP_ASSERT(_mainWindowSettings);

        if (!settings.StartLoadObject(MainWindowStr))
        {
            KMP_LOG_CORE_WARN("WindowBackendGlfw: main window settings were not found");
        }
        else
        {
            _mainWindowSettings->width = settings.GetInt(Window::WidthStr, Window::DefaultWidth);
            _mainWindowSettings->height = settings.GetInt(Window::HeightStr, Window::DefaultHeight);
            _mainWindowSettings->windowedWidth = settings.GetInt(Window::WindowedWidthStr, Window::DefaultWidth);
            _mainWindowSettings->windowedHeight = settings.GetInt(Window::WindowedHeightStr, Window::DefaultHeight);
            _mainWindowSettings->screenMode = Window::StringToMode(settings.GetString(Window::ScreenModeStr, Window::WindowedModeStr));
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
        const auto windowsCount = settings.StartLoadArray(AuxWindowsStr);
        for (auto i = 0; i < windowsCount; i++)
        {
            settings.StartLoadObject(i);

            const auto windowName = settings.GetString(Window::NameStr, "");
            if (windowName.empty())
            {
                KMP_LOG_CORE_ERROR("WindowBackendGlfw: loading settings for unnamed window is prohibited, current object would be skipped, remove it from settings");
            }
            else
            {
                auto windowSettings = CreateUPtr<Window::WindowSettings>(windowName);
                windowSettings->width = settings.GetInt(Window::WidthStr, Window::DefaultWidth);
                windowSettings->height = settings.GetInt(Window::HeightStr, Window::DefaultHeight);
                windowSettings->windowedWidth = settings.GetInt(Window::WindowedWidthStr, Window::DefaultWidth);
                windowSettings->windowedHeight = settings.GetInt(Window::WindowedHeightStr, Window::DefaultHeight);
                windowSettings->screenMode = Window::StringToMode(settings.GetString(Window::ScreenModeStr, Window::WindowedModeStr));
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
