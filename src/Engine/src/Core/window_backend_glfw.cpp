#include "Kmplete/Core/window_backend_glfw.h"
#include "Kmplete/Core/window_glfw.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"

#include <GLFW/glfw3.h>

#ifdef CreateWindow
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
                for (auto i = 0; i < modesCount; i++)
                {
                    auto mode = modes[i];
                    outputModes.push_back(WindowBackend::MonitorVideoMode{ .width = mode.width, .height = mode.height, .refreshRate = mode.refreshRate });
                }
            }

            return outputModes;
        }
        //--------------------------------------------------------------------------
    }


    WindowBackendGlfw::WindowBackendGlfw()
    {
        Initialize();

        Log::CoreTrace("WindowBackendGlfw: created");
    }
    //--------------------------------------------------------------------------

    WindowBackendGlfw::~WindowBackendGlfw()
    {
        Finalize();

        Log::CoreTrace("WindowBackendGlfw: destroyed");
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::Initialize()
    {
        if (!glfwInit())
        {
            const char* description;
            const auto errorCode = glfwGetError(&description);
            Log::CoreCritical("WindowBackendGlfw: initialization error: code '{}', description '{}'", errorCode, description ? description : "");
            throw std::exception("WindowBackendGlfw initialization failed");
        }
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::Finalize()
    {
        if (glfwGetCurrentContext())
        {
            Log::CoreWarn("WindowBackendGlfw: some window's context is still current");
        }

        _windowsSettings.clear();
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    Ptr<Window> WindowBackendGlfw::CreateWindow(const std::string& windowName)
    {
        if (windowName.empty())
        {
            Log::CoreError("WindowBackendGlfw: creation of unnamed windows is prohibited");
            return nullptr;
        }

        if (_windowsSettings.contains(windowName))
        {
            Log::CoreInfo("WindowBackendGlfw: creating window '{}' with previously loaded settings", windowName);
            try
            {
                const auto newWindow = CreatePtr<WindowGlfw>(_windowsSettings[windowName]);
                return newWindow;
            }
            catch (const std::exception& e)
            {
                Log::CoreError("WindowBackendGlfw: error creating window '{}', message: '{}'", windowName, e.what());
                return nullptr;
            }
        }
        else
        {
            Log::CoreInfo("WindowBackendGlfw: creating window '{}' with default settings", windowName);
            auto windowSettings = CreatePtr<Window::WindowSettings>(windowName);
            _windowsSettings[windowName] = windowSettings;

            try
            {
                const auto newWindow = CreatePtr<WindowGlfw>(windowSettings);
                return newWindow;
            }
            catch (const std::exception& e)
            {
                Log::CoreError("WindowBackendGlfw: error creating window '{}', message: '{}'", windowName, e.what());
                return nullptr;
            }
        }
    }
    //--------------------------------------------------------------------------

    Ptr<Window> WindowBackendGlfw::CreateWindow(const Ptr<Window::WindowSettings> windowSettings)
    {
        try
        {
            KMP_CHECK(windowSettings && !windowSettings->name.empty()).Exception();

            const auto& windowName = windowSettings->name;
            if (_windowsSettings.contains(windowName))
            {
                Log::CoreWarn("WindowBackendGlfw: window '{}' will be created, but settings already contains this name and will be overriden", windowName);
            }
            else
            {
                Log::CoreInfo("WindowBackendGlfw: window '{}' will be created with provided settings", windowName);
                _windowsSettings.insert({ windowName, windowSettings });
            }

            return CreatePtr<WindowGlfw>(windowSettings);
        }
        catch (const std::exception& e)
        {
            Log::CoreError("WindowBackendGlfw: error creating window '{}', message: '{}'", windowSettings->name, e.what());
            return nullptr;
        }
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
            for (auto i = 0; i < count; i++)
            {
                const auto monitor = monitors[i];
                if (monitor)
                {
                    names.push_back(glfwGetMonitorName(monitor));
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

    void WindowBackendGlfw::SaveSettings(const Ptr<Settings> settings) const
    {
        KMP_ASSERT(settings);

        settings->StartSaveObject(WindowBackendSettingsEntryName);
        settings->StartSaveArray(WindowsStr);
        int index = 0;
        for (const auto& windowEntry : _windowsSettings)
        {
            const auto windowSettings = windowEntry.second;
            KMP_ASSERT(windowSettings);

            settings->StartSaveObject(index);

            settings->SaveString(Window::NameStr, windowSettings->name);
            settings->SaveUInt(Window::WidthStr, windowSettings->width);
            settings->SaveUInt(Window::HeightStr, windowSettings->height);
            settings->SaveUInt(Window::WindowedWidthStr, windowSettings->windowedWidth);
            settings->SaveUInt(Window::WindowedHeightStr, windowSettings->windowedHeight);
            settings->SaveString(Window::ScreenModeStr, windowSettings->screenMode);
            settings->SaveBool(Window::VSyncStr, windowSettings->vSync);
            settings->SaveBool(Window::UpdateContinuouslyStr, windowSettings->updateContinuously);

            settings->EndSaveObject();
            ++index;
        }

        settings->EndSaveArray();
        settings->EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadSettings(const Ptr<Settings> settings)
    {
        KMP_ASSERT(settings);

        settings->StartLoadObject(WindowBackendSettingsEntryName);
        const auto windowsCount = settings->StartLoadArray(WindowsStr);
        for (auto i = 0; i < windowsCount; i++)
        {
            settings->StartLoadObject(i);

            const auto windowName = settings->GetString(Window::NameStr, "");
            if (windowName.empty())
            {
                Log::CoreError("WindowBackendGlfw: loading settings for unnamed window is prohibited, current object would be skipped, remove it from settings");
            }
            else
            {
                auto windowSettings = CreatePtr<Window::WindowSettings>(windowName);
                windowSettings->width = settings->GetUInt(Window::WidthStr, Window::DefaultWidth);
                windowSettings->height = settings->GetUInt(Window::HeightStr, Window::DefaultHeight);
                windowSettings->windowedWidth = settings->GetUInt(Window::WindowedWidthStr, Window::DefaultWidth);
                windowSettings->windowedHeight = settings->GetUInt(Window::WindowedHeightStr, Window::DefaultHeight);
                windowSettings->screenMode = settings->GetString(Window::ScreenModeStr, Window::WindowedModeStr);
                windowSettings->vSync = settings->GetBool(Window::VSyncStr, true);
                windowSettings->updateContinuously = settings->GetBool(Window::UpdateContinuouslyStr, true);
                _windowsSettings.insert({ windowName, windowSettings });
            }

            settings->EndLoadObject();
        }

        settings->EndLoadArray();
        settings->EndLoadObject();
    }
    //--------------------------------------------------------------------------
}

#ifdef KMP_UNDEF_CreateWindow
#pragma pop_macro("CreateWindow")
#undef KMP_UNDEF_CreateWindow
#endif