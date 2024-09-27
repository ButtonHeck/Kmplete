#include "Kmplete/Core/window_backend.h"
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

TEST_CASE("Window backend single window", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    const auto mainWindow = windowBackend->CreateWindow("Main");
    REQUIRE(mainWindow);
    REQUIRE(mainWindow.use_count() == 2);
    REQUIRE(mainWindow->GetImplPointer());
    REQUIRE(mainWindow->GetName() == std::string("Main"));

    const auto size = mainWindow->GetSize();
    REQUIRE(size.first > 0); 
    REQUIRE(size.second > 0);

    const auto windowedSize = mainWindow->GetWindowedSize();
    REQUIRE(windowedSize.first > 0); 
    REQUIRE(windowedSize.second > 0);

    mainWindow->SetVSync(true);
    REQUIRE(mainWindow->IsVSync());
    mainWindow->SetVSync(false);
    REQUIRE_FALSE(mainWindow->IsVSync());

    mainWindow->SetUpdatedContinuously(true);
    REQUIRE(mainWindow->IsUpdatedContinuously());
    mainWindow->SetUpdatedContinuously(false);
    REQUIRE_FALSE(mainWindow->IsUpdatedContinuously());
}
//--------------------------------------------------------------------------

struct Callback
{
    Callback(const Kmplete::Ptr<Kmplete::Window> window)
        : window(window)
    {
        window->SetEventCallback(KMP_BIND(Callback::WindowCallback));
    }

    void WindowCallback(Kmplete::Event& evt)
    {
        Kmplete::EventDispatcher dispatcher(evt);

        dispatcher.Dispatch<Kmplete::WindowCloseEvent>([this](Kmplete::WindowCloseEvent&)
            {
                window->SetShouldClose(true);
                return true;
            });
        dispatcher.Dispatch<Kmplete::KeyPressEvent>([this](Kmplete::KeyPressEvent& evt)
            {
                if (evt.GetKeyCode() == Kmplete::Key::Y)
                {
                    conditionOk = true;
                }
                window->SetShouldClose(true);
                return true;
            });
    }

    Kmplete::Ptr<Kmplete::Window> window;
    bool conditionOk = false;
};

TEST_CASE("Window backend UpdateContinuously ON", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window UpdateContinuously ON test", "Press Y if window title changes repeatedly without any conditions, otherwise press any other key", Kmplete::FileDialogs::MessageChoice::Ok);

    const auto mainWindow = windowBackend->CreateWindow("Main");
    REQUIRE(mainWindow);
    mainWindow->SetTitle("Main Window");
    mainWindow->SetUpdatedContinuously(true);
    Callback mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow->ShouldClose())
    {
        mainWindow->ProcessEvents();
        mainWindow->SwapBuffers();

        // every SetTitle generates new event that can't be captured via callback, thus it should be sparsed
        if (titleCount++ % 10 == 0)
            mainWindow->SetTitle(std::to_string(titleCount / 10));
    }

    REQUIRE(mainWindowCb.conditionOk);
}

TEST_CASE("Window backend UpdateContinuously OFF", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window UpdateContinuously OFF test", "Press Y if window title changes only on some events, otherwise press any other key", Kmplete::FileDialogs::MessageChoice::Ok);

    const auto mainWindow = windowBackend->CreateWindow("Main");
    REQUIRE(mainWindow);
    mainWindow->SetTitle("Main Window");
    mainWindow->SetUpdatedContinuously(false);
    Callback mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow->ShouldClose())
    {
        mainWindow->ProcessEvents();
        mainWindow->SwapBuffers();

        // every SetTitle generates new event that can't be captured via callback, thus it should be sparsed
        if (titleCount++ % 10 == 0)
            mainWindow->SetTitle(std::to_string(titleCount / 10));
    }

    REQUIRE(mainWindowCb.conditionOk);
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend VSync ON", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window VSync ON test", "Press Y if window title changes once a second (for 60Hz)", Kmplete::FileDialogs::MessageChoice::Ok);

    const auto mainWindow = windowBackend->CreateWindow("Main");
    REQUIRE(mainWindow);
    mainWindow->SetTitle("Main Window");
    mainWindow->SetVSync(true);
    Callback mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow->ShouldClose())
    {
        mainWindow->ProcessEvents();
        mainWindow->SwapBuffers();

        if (titleCount++ % 60 == 0)
            mainWindow->SetTitle(std::to_string(titleCount / 60));
    }

    REQUIRE(mainWindowCb.conditionOk);
}

TEST_CASE("Window backend VSync OFF", "[core][window_backend][window]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Window VSync OFF test", "Press Y if window title changes fast", Kmplete::FileDialogs::MessageChoice::Ok);

    const auto mainWindow = windowBackend->CreateWindow("Main");
    REQUIRE(mainWindow);
    mainWindow->SetTitle("Main Window");
    mainWindow->SetVSync(false);
    Callback mainWindowCb(mainWindow);

    int titleCount = 0;
    while (!mainWindow->ShouldClose())
    {
        mainWindow->ProcessEvents();
        mainWindow->SwapBuffers();

        if (titleCount++ % 60 == 0)
            mainWindow->SetTitle(std::to_string(titleCount / 60));
    }

    REQUIRE(mainWindowCb.conditionOk);
}
//--------------------------------------------------------------------------

//TEST_CASE("Window backend multiple windows", "[core][window_backend][window]")
//{
//    const auto windowBackend = Kmplete::WindowBackend::Create();
//    REQUIRE(windowBackend);
//
//    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("Multiple windows test", "Make sure both windows can be resized, moved, hid. Then close both windows", Kmplete::FileDialogs::MessageChoice::Ok);
//
//    const auto mainWindow = windowBackend->CreateWindow("Main");
//    REQUIRE(mainWindow);
//    mainWindow->SetTitle("Main Window");
//    Callback mainWindowCb(mainWindow);
//
//    const auto auxWindow = windowBackend->CreateWindow("Aux");
//    REQUIRE(auxWindow);
//    auxWindow->SetTitle("Aux window");
//    Callback auxWindowCb(auxWindow);
//
//    while (!mainWindow->ShouldClose() || !auxWindow->ShouldClose())
//    {
//        if (!mainWindow->ShouldClose())
//        {
//            mainWindow->ProcessEvents();
//            mainWindow->SwapBuffers();
//        }
//        if (!auxWindow->ShouldClose())
//        {
//            auxWindow->ProcessEvents();
//            auxWindow->SwapBuffers();
//        }
//    }
//
//    SUCCEED();
//}
//--------------------------------------------------------------------------