#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>


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
//--------------------------------------------------------------------------

struct TestStartResult
{
    Kmplete::UPtr<Kmplete::WindowBackend> windowBackend = nullptr;
    Kmplete::Window& mainWindow;
    bool mainWindowNameIsMain = false;

    static TestStartResult InitializeTestData()
    {
        auto windowBackend = Kmplete::WindowBackend::Create();
        auto& mainWindow = windowBackend->CreateMainWindow();

        return TestStartResult{ std::move(windowBackend), mainWindow, mainWindow.GetName() == "Main" };
    }
};
//--------------------------------------------------------------------------


TEST_CASE("Main window DPI scale", "[core][window_backend][window]")
{
    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    auto dpi = 0.0f;
    REQUIRE_NOTHROW(dpi = mainWindow.GetDPIScale());
    REQUIRE(dpi != 0.0f);
}
//--------------------------------------------------------------------------

TEST_CASE("Main window default sizes test", "[core][window_backend][window]")
{
    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    const auto size = mainWindow.GetSize();
    REQUIRE(size.x > 0);
    REQUIRE(size.y > 0);

    const auto windowedSize = mainWindow.GetWindowedSize();
    REQUIRE(windowedSize.x > 0);
    REQUIRE(windowedSize.y > 0);
}
//--------------------------------------------------------------------------

TEST_CASE("Main window UpdateContinuously ON", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window UpdateContinuously ON test",
        "Press Y if window title changes repeatedly without any conditions, otherwise press any other key",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.SetUpdatedContinuously(true);
    REQUIRE(mainWindow.IsUpdatedContinuously());
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

TEST_CASE("Main window UpdateContinuously OFF", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window UpdateContinuously OFF test",
        "Press Y if window title changes only on some events, otherwise press any other key",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.SetUpdatedContinuously(false);
    REQUIRE_FALSE(mainWindow.IsUpdatedContinuously());
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

TEST_CASE("Main window VSync ON", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window VSync ON test",
        "Press Y if window title changes once a second (for 60Hz)",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.SetVSync(true);
    REQUIRE(mainWindow.IsVSync());
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

TEST_CASE("Main window VSync OFF", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window VSync OFF test",
        "Press Y if window title changes fast",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.SetVSync(false);
    REQUIRE_FALSE(mainWindow.IsVSync());
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

TEST_CASE("Multiple windows test", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Multiple windows test",
        "Make sure both windows can be resized, moved, hid. Then close both windows",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

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

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window screen mode test",
        "Press F (fullscreen), W (Windowed) and E (Windowed fullscreen). Change screen mode and if everything is ok - press Y, otherwise - any other key",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend&& windowNameIsMain));

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
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window with premade settings",
        "Press Y if window is 200x200, otherwise - any other key",
        Kmplete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("SomeWindow");
    REQUIRE(settings.name == "SomeWindow");
    settings.size = {200, 200};
    settings.windowedSize = {200, 200};
    settings.vSync = true;
    settings.updateContinuously = true;

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}

TEST_CASE("Window create via existing invalid WindowSettings", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window with invalid settings",
        "Close this window",
        Kmplete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("");
    REQUIRE(settings.name == "");

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings)); //expect exception during creation but catching it in window backend
    REQUIRE_FALSE(window);

    Kmplete::Window::WindowSettings settings2 = Kmplete::Window::WindowSettings("ValidName-InvalidWidth");
    REQUIRE(settings2.name == "ValidName-InvalidWidth");
    settings2.size = {65000, 200};

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

TEST_CASE("Window resizable OFF", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window resizable OFF test",
        "Press Y if window cannot be resized",
        Kmplete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings("SomeWindow");
    settings.size = {400, 400};
    settings.windowedSize = {400, 400};
    settings.vSync = true;
    settings.updateContinuously = true;
    settings.resizable = false;

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    while (!window->ShouldClose())
    {
        window->ProcessEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window decorated OFF", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window decorated OFF test",
        "Press Y if window is not decorated",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.SetDecorated(false);

    WindowCallbackUserSingleCondition windowCb(mainWindow);

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window SetPosition", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window SetPosition test",
        "Press Y if window is at [500, 50] coordinates",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    WindowCallbackUserSingleCondition windowCb(mainWindow);

    mainWindow.SetPosition(500, 50);

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window GetPosition", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window GetPosition test",
        "Move window and press Y if window position (shown as title) seems correct",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    WindowCallbackUserSingleCondition windowCb(mainWindow);

    int x = 0;
    int y = 0;

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();

        const auto position = mainWindow.GetPosition();
        x = position.x;
        y = position.y;

        const Kmplete::String title = Kmplete::String("window position = [") + std::to_string(x) + ", " + std::to_string(y) + "]";
        mainWindow.SetTitle(title.c_str());
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window center at current screen", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window alwaysOnTop test",
        "Press Y if window is at the current screen's center",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.PositionAtCurrentScreenCenter();

    WindowCallbackUserSingleCondition windowCb(mainWindow);

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window is alwaysOnTop", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window alwaysOnTop test",
        "Press Y if window is always on top of other windows",
        Kmplete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    mainWindow.SetAlwaysOnTop(true);

    WindowCallbackUserSingleCondition windowCb(mainWindow);

    while (!mainWindow.ShouldClose())
    {
        mainWindow.ProcessEvents();
        mainWindow.SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------