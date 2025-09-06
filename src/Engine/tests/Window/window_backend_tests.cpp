#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Window backend no-window", "[core][window_backend]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    const auto monitorsCount = windowBackend->GetMonitorCount();
    REQUIRE(monitorsCount > 0);

    const auto monitorNames = windowBackend->GetMonitorNames();
    REQUIRE(!monitorNames.empty());

    bool monitorNamesNotEmpty = true;
    for (const auto& monitorName : monitorNames)
    {
        if (monitorName.empty())
        {
            monitorNamesNotEmpty = false;
            break;
        }
    }
    REQUIRE(monitorNamesNotEmpty);

    const auto primaryMonitorModes = windowBackend->GetPrimaryMonitorVideoModes();
    REQUIRE(!primaryMonitorModes.empty());

    auto primaryMonitorModesDataValid = true;
    for (const auto& monitorMode : primaryMonitorModes)
    {
        if (monitorMode.width <= 0 || monitorMode.height <= 0 || monitorMode.refreshRate <= 0)
        {
            primaryMonitorModesDataValid = false;
            break;
        }
    }
    REQUIRE(primaryMonitorModesDataValid);

    // assume previous test for primary monitor is ok
    const auto firstMonitorVideoModes = windowBackend->GetMonitorVideoModes(0);
    REQUIRE(!firstMonitorVideoModes.empty());

    auto firstMonitorVideoModesDataValid = true;
    for (const auto& monitorMode : firstMonitorVideoModes)
    {
        if (monitorMode.width <= 0 || monitorMode.height <= 0 || monitorMode.refreshRate <= 0)
        {
            firstMonitorVideoModesDataValid = false;
            break;
        }
    }
    REQUIRE(firstMonitorVideoModesDataValid);

    auto nonExistentMonitorVideoModes = std::vector<Kmplete::WindowBackend::MonitorVideoMode>();
    REQUIRE_NOTHROW(nonExistentMonitorVideoModes = windowBackend->GetMonitorVideoModes(monitorsCount));
    REQUIRE(nonExistentMonitorVideoModes.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend primary monitor DPI scale", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    auto dpi = 0.0f;
    REQUIRE_NOTHROW(dpi = windowBackend->GetPrimaryMonitorDPIScale());
    REQUIRE(dpi != 0.0f);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend window DPI scale", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    auto& mainWindow = windowBackend->CreateMainWindow();
    REQUIRE(mainWindow.GetName() == "Main");

    auto dpi = 0.0f;
    REQUIRE_NOTHROW(dpi = mainWindow.GetDPIScale());
    REQUIRE(dpi != 0.0f);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend single window", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    auto& mainWindow = windowBackend->CreateMainWindow();
    REQUIRE(mainWindow.GetName() == "Main");

    const auto [width, height] = mainWindow.GetSize();
    REQUIRE(width > 0);
    REQUIRE(height > 0);

    const auto [windowedWidth, windowedHeight] = mainWindow.GetWindowedSize();
    REQUIRE(windowedWidth > 0);
    REQUIRE(windowedHeight > 0);

    mainWindow.SetVSync(true);
    REQUIRE(mainWindow.IsVSync());
    mainWindow.SetVSync(false);
    REQUIRE_FALSE(mainWindow.IsVSync());

    mainWindow.SetUpdatedContinuously(true);
    REQUIRE(mainWindow.IsUpdatedContinuously());
    mainWindow.SetUpdatedContinuously(false);
    REQUIRE_FALSE(mainWindow.IsUpdatedContinuously());
}
//--------------------------------------------------------------------------

struct WindowCallbackUserSingleCondition
{
    WindowCallbackUserSingleCondition(Kmplete::Window& window)
        : window(window)
    {
        window.SetEventCallback(KMP_BIND(WindowCallbackUserSingleCondition::Callback));
    }

    void Callback(Kmplete::Event& evt)
    {
        Kmplete::EventDispatcher dispatcher(evt);

        dispatcher.Dispatch<Kmplete::WindowCloseEvent>([this](Kmplete::WindowCloseEvent&)
            {
                window.SetShouldClose(true);
                return true;
            });
        dispatcher.Dispatch<Kmplete::KeyPressEvent>([this](Kmplete::KeyPressEvent& evt)
            {
                if (evt.GetKeyCode() == Kmplete::Key::Y)
                {
                    conditionOk = true;
                }
                window.SetShouldClose(true);
                return true;
            });
    }

    Kmplete::Window& window;
    bool conditionOk = false;
};

TEST_CASE("Window backend UpdateContinuously ON", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window UpdateContinuously ON test", 
                                                                     "Press Y if window title changes repeatedly without any conditions, otherwise press any other key", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    auto& mainWindow = windowBackend->CreateMainWindow();
    mainWindow.SetTitle("Main Window");
    mainWindow.SetUpdatedContinuously(true);
    WindowCallbackUserSingleCondition mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();

        // every SetTitle generates new event that can't be captured via callback, thus it should be sparsed
        if (titleCount++ % 10 == 0)
            mainWindow.SetTitle(std::to_string(titleCount / 10).c_str());
    }

    REQUIRE(mainWindowCb.conditionOk);
}

TEST_CASE("Window backend UpdateContinuously OFF", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window UpdateContinuously OFF test", 
                                                                     "Press Y if window title changes only on some events, otherwise press any other key", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    auto& mainWindow = windowBackend->CreateMainWindow();
    mainWindow.SetTitle("Main Window");
    mainWindow.SetUpdatedContinuously(false);
    WindowCallbackUserSingleCondition mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();

        // every SetTitle generates new event that can't be captured via callback, thus it should be sparsed
        if (titleCount++ % 10 == 0)
            mainWindow.SetTitle(std::to_string(titleCount / 10).c_str());
    }

    REQUIRE(mainWindowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend VSync ON", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window VSync ON test", 
                                                                     "Press Y if window title changes once a second (for 60Hz)", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    auto& mainWindow = windowBackend->CreateMainWindow();
    mainWindow.SetTitle("Main Window");
    mainWindow.SetVSync(true);
    WindowCallbackUserSingleCondition mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();

        if (titleCount++ % 60 == 0)
            mainWindow.SetTitle(std::to_string(titleCount / 60).c_str());
    }

    REQUIRE(mainWindowCb.conditionOk);
}

TEST_CASE("Window backend VSync OFF", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window VSync OFF test", 
                                                                     "Press Y if window title changes fast", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    auto& mainWindow = windowBackend->CreateMainWindow();
    mainWindow.SetTitle("Main Window");
    mainWindow.SetVSync(false);
    WindowCallbackUserSingleCondition mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();

        if (titleCount++ % 60 == 0)
            mainWindow.SetTitle(std::to_string(titleCount / 60).c_str());
    }

    REQUIRE(mainWindowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend multiple windows", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Multiple windows test", 
                                                                     "Make sure both windows can be resized, moved, hid. Then close both windows", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    auto& mainWindow = windowBackend->CreateMainWindow();
    mainWindow.SetTitle("Main Window");

    auto auxWindow = windowBackend->CreateAuxWindow("Aux");
    REQUIRE(auxWindow);
    auxWindow->SetTitle("Aux window");

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();

        if (auxWindow && !auxWindow->ShouldClose())
        {
            auxWindow->ProcessEvents();
            auxWindow->SwapBuffers();
        }
        else
        {
            windowBackend->DestroyAuxWindow("Aux");
            auxWindow = nullptr;
        }
    }

    SUCCEED();
}
//--------------------------------------------------------------------------

TEST_CASE("Window screen mode change", "[core][window_backend][window]")
{
    struct WindowCallbackUserSM
    {
        WindowCallbackUserSM(Kmplete::Window& window)
            : window(window)
        {
            window.SetEventCallback(KMP_BIND(WindowCallbackUserSM::Callback));
        }

        void Callback(Kmplete::Event& evt)
        {
            Kmplete::EventDispatcher dispatcher(evt);

            dispatcher.Dispatch<Kmplete::WindowCloseEvent>([this](Kmplete::WindowCloseEvent&)
                {
                    window.SetShouldClose(true);
                    return true;
                });
            dispatcher.Dispatch<Kmplete::KeyPressEvent>([this](Kmplete::KeyPressEvent& evt)
                {
                    if (evt.GetKeyCode() == Kmplete::Key::F)
                    {
                        window.SetScreenMode(Kmplete::Window::ScreenMode::Fullscreen);
                        fullscreenOk = (window.GetScreenMode() == Kmplete::Window::ScreenMode::Fullscreen);
                    }
                    else if (evt.GetKeyCode() == Kmplete::Key::W)
                    {
                        window.SetScreenMode(Kmplete::Window::ScreenMode::Windowed);
                        windowedOk = (window.GetScreenMode() == Kmplete::Window::ScreenMode::Windowed);
                    }
                    else if (evt.GetKeyCode() == Kmplete::Key::E)
                    {
                        window.SetScreenMode(Kmplete::Window::ScreenMode::WindowedFullscreen);
                        windowedFullscreenOk = (window.GetScreenMode() == Kmplete::Window::ScreenMode::WindowedFullscreen);
                    }
                    else if (evt.GetKeyCode() == Kmplete::Key::Y)
                    {
                        allOk = true;
                        window.SetShouldClose(true);
                    }
                    else
                    {
                        allOk = false;
                        window.SetShouldClose(true);
                    }

                    return true;
                });
        }

        Kmplete::Window& window;
        bool fullscreenOk = false;
        bool windowedOk = false;
        bool windowedFullscreenOk = false;
        bool allOk = false;
    };

    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window screen mode test", 
                                                                     "Press F (fullscreen), W (Windowed) and E (Windowed fullscreen). Change screen mode and if everything is ok - press Y, otherwise - any other key", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    auto& mainWindow = windowBackend->CreateMainWindow();
    mainWindow.SetTitle("Main Window");
    WindowCallbackUserSM windowCb(mainWindow);

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();
    }

    REQUIRE(windowCb.fullscreenOk);
    REQUIRE(windowCb.windowedOk);
    REQUIRE(windowCb.windowedFullscreenOk);
    REQUIRE(windowCb.allOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window create via existing valid WindowSettings", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("SomeWindow");
    REQUIRE(settings.name == "SomeWindow");
    settings.width = 200;
    settings.height = 200;
    settings.windowedWidth = 200;
    settings.windowedHeight = 200;
    settings.vSync = true;
    settings.updateContinuously = true;
    
    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window with premade settings", 
                                                                     "Press Y if window is 200x200, otherwise - any other key", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}

TEST_CASE("Window create via existing invalid WindowSettings", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("");
    REQUIRE(settings.name == "");

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings)); //expect exception during creation but catching it in window backend
    REQUIRE_FALSE(window);


    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window with invalid settings", 
                                                                     "Close this window", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    Kmplete::Window::WindowSettings settings2 = Kmplete::Window::WindowSettings("ValidName-InvalidWidth");
    REQUIRE(settings2.name == "ValidName-InvalidWidth");
    settings2.width = 65000;
    settings2.height = 200;

    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings2));
    REQUIRE(window);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    SUCCEED();
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend resizable OFF", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("SomeWindow");
    settings.width = 400;
    settings.height = 400;
    settings.windowedWidth = 400;
    settings.windowedHeight = 400;
    settings.vSync = true;
    settings.updateContinuously = true;
    settings.resizable = false;

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window resizable OFF test", 
                                                                     "Press Y if window cannot be resized", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend decorated OFF", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("SomeWindow");
    settings.width = 400;
    settings.height = 400;
    settings.windowedWidth = 400;
    settings.windowedHeight = 400;
    settings.vSync = true;
    settings.updateContinuously = true;
    settings.decorated = false;

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window decorated OFF test", 
                                                                     "Press Y if window is not decorated", 
                                                                     Kmplete::FileDialogs::MessageChoice::Ok);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend SetPosition", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("SomeWindow");
    settings.width = 400;
    settings.height = 400;
    settings.windowedWidth = 400;
    settings.windowedHeight = 400;
    settings.vSync = true;
    settings.updateContinuously = true;
    settings.decorated = true;

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window SetPosition test",
        "Press Y if window is at [500, 50] coordinates",
        Kmplete::FileDialogs::MessageChoice::Ok);

    window->SetPosition(500, 50);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------