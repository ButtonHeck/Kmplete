#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/event_dispatcher.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>


using namespace Kmplete;
struct WindowCallbackUserSingleCondition
{
    WindowCallbackUserSingleCondition(Kmplete::Window& window)
        : window(window)
    {
        window.SetEventCallback(KMP_BIND(WindowCallbackUserSingleCondition::Callback));
    }

    void Callback(Kmplete::Events::Event& evt)
    {
        if (evt.GetTypeID() == "WindowCloseEvent"_sid)
        {
            window.SetShouldClose(true);
        }
        else if (evt.GetTypeID() == "KeyPressEvent"_sid)
        {
            auto keyEvent = static_cast<Events::KeyPressEvent&>(evt);
            if (keyEvent.GetKeyCode() == Kmplete::Input::Key::Y)
            {
                conditionOk = true;
            }
            window.SetShouldClose(true);
        }
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
        mainWindow.FetchEvents();
        mainWindow.SwapBuffers();

        if (auxWindow && !auxWindow->ShouldClose())
        {
            auxWindow->FetchEvents();
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


TEST_CASE("Window create via existing valid WindowSettings", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window with premade settings",
        "Press Y if window is 200x200, otherwise - any other key",
        Kmplete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings;
    settings.name = "Some window";
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
        window->FetchEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------


TEST_CASE("Window create via existing invalid WindowSettings", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window with invalid settings",
        "Close this window",
        Kmplete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    Kmplete::Window::WindowSettings settings;
    REQUIRE(settings.name == "");

    Kmplete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings)); //expect exception during creation but catching it in window backend
    REQUIRE_FALSE(window);

    Kmplete::Window::WindowSettings settings2;
    settings2.name = "ValidName-InvalidWidth";
    settings2.size = {65000, 200};

    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings2));
    REQUIRE(window);

    while (!window->ShouldClose())
    {
        window->FetchEvents();
        window->SwapBuffers();
    }

    SUCCEED();
}
//--------------------------------------------------------------------------