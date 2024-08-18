#include "Kmplete/Core/window_backend_glfw.h"
#include "Kmplete/Core/window_glfw.h"
#include "Kmplete/Core/log.h"
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
        : _mainWindow(CreatePtr<WindowGlfw>())
    {}
    //--------------------------------------------------------------------------

    bool WindowBackendGlfw::Initialize(const Ptr<Settings> settings)
    {
        LoadSettings(settings);

        if (!glfwInit())
        {
            const char* description;
            const auto errorCode = glfwGetError(&description);
            Log::CoreCritical("WindowBackendGlfw: initialization error: code '{}', description '{}'", errorCode, description ? description : "");
            return false;
        }

        if (!_mainWindow->Initialize(settings))
        {
            Log::CoreCritical("WindowBackendGlfw: main window initialization failed");
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::Finalize() const
    {
        KMP_ASSERT(_mainWindow);
        _mainWindow->Finalize();
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    Ptr<Window> WindowBackendGlfw::GetMainWindow()
    {
        return _mainWindow;
    }
    //--------------------------------------------------------------------------

    int WindowBackendGlfw::GetMonitorCount() const
    {
        int count = 0;
        glfwGetMonitors(&count);
        return count;
    }
    //--------------------------------------------------------------------------

    std::vector<std::string> WindowBackendGlfw::GetMonitorNames() const
    {
        int count = 0;
        const auto monitors = glfwGetMonitors(&count);

        std::vector<std::string> names;
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
        settings->StartSaveObject(WindowBackendSettingsEntryName);
        settings->StartSaveObject(MainWindowStr);
        _mainWindow->SaveSettings(settings);
        settings->EndSaveObject();
        settings->EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::LoadSettings(const Ptr<Settings> settings)
    {
        settings->StartLoadObject(WindowBackendSettingsEntryName);
        settings->StartLoadObject(MainWindowStr);
        _mainWindow->LoadSettings(settings);
        settings->EndLoadObject();
        settings->EndLoadObject();
    }
    //--------------------------------------------------------------------------
}

#ifdef KMP_UNDEF_CreateWindow
#pragma pop_macro("CreateWindow")
#undef KMP_UNDEF_CreateWindow
#endif