#include "Kmplete/Core/window_application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

namespace Kmplete
{
    class TestWindowApplication : public WindowApplication
    {
    public:
        TestWindowApplication(const std::string& settingsFilePath, const std::string& defaultSettingsName)
            : WindowApplication(settingsFilePath, defaultSettingsName)
        {
            Initialize();
        }

        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override
        {
            return std::string("TestApplication");
        }

        void Run() override
        {
            while (!_completed)
            {
                _mainWindow->ProcessEvents();
                _mainWindow->SwapBuffers();
            }
        }

        void Initialize()
        {
            _mainWindow->SetEventCallback(KMP_BIND(TestWindowApplication::OnEvent));
        }

        bool IsKeyPressEventInvoked() const { return _keyPressEventInvoked; }
        bool IsKeyReleaseEventInvoked() const { return _keyReleaseEventInvoked; }
        bool IsKeyCharEventInvoked() const { return _keyCharEventInvoked; }
        bool IsWindowApplicationKeyPressEventInvoked() const { return _windowApplicationKeyPressEventInvoked; }
        bool IsWindowApplicationKeyReleaseEventInvoked() const { return _windowApplicationKeyReleaseEventInvoked; }
        bool IsWindowApplicationKeyCharEventInvoked() const { return _windowApplicationKeyCharEventInvoked; }

        bool IsMouseMoveEventInvoked() const { return _mouseMoveEventInvoked; }
        bool IsMouseScrollEventInvoked() const { return _mouseScrollEventInvoked; }
        bool IsMouseButtonPressEventInvoked() const { return _mouseButtonPressEventInvoked; }
        bool IsMouseButtonReleaseEventInvoked() const { return _mouseButtonReleaseEventInvoked; }
        bool IsWindowApplicationMouseMoveEventInvoked() const { return _windowApplicationMouseMoveEventInvoked; }
        bool IsWindowApplicationMouseScrollEventInvoked() const { return _windowApplicationMouseScrollEventInvoked; }
        bool IsWindowApplicationMouseButtonPressEventInvoked() const { return _windowApplicationMouseButtonPressEventInvoked; }
        bool IsWindowApplicationMouseButtonReleaseEventInvoked() const { return _windowApplicationMouseButtonReleaseEventInvoked; }

        bool IsWindowCloseEventInvoked() const { return _windowCloseEventInvoked; }
        bool IsWindowMoveEventInvoked() const { return _windowMoveEventInvoked; }
        bool IsWindowResizeEventInvoked() const { return _windowResizeEventInvoked; }
        bool IsWindowFocusEventInvoked() const { return _windowFocusEventInvoked; }
        bool IsWindowIconifyEventInvoked() const { return _windowIconifyEventInvoked; }
        bool IsWindowFramebufferRefreshEventInvoked() const { return _windowFramebufferRefreshEventInvoked; }
        bool IsWindowFramebufferResizeEventInvoked() const { return _windowFramebufferResizeEventInvoked; }
        bool IsWindowApplicationWindowCloseEventInvoked() const { return _windowApplicationWindowCloseEventInvoked; }
        bool IsWindowApplicationWindowMoveEventInvoked() const { return _windowApplicationWindowMoveEventInvoked; }
        bool IsWindowApplicationWindowResizeEventInvoked() const { return _windowApplicationWindowResizeEventInvoked; }
        bool IsWindowApplicationWindowFocusEventInvoked() const { return _windowApplicationWindowFocusEventInvoked; }
        bool IsWindowApplicationWindowIconifyEventInvoked() const { return _windowApplicationWindowIconifyEventInvoked; }
        bool IsWindowApplicationWindowFramebufferRefreshEventInvoked() const { return _windowApplicationWindowFramebufferRefreshEventInvoked; }
        bool IsWindowApplicationWindowFramebufferResizeEventInvoked() const { return _windowApplicationWindowFramebufferResizeEventInvoked; }

    protected:
        void OnEvent(Event& event) override
        {
            EventDispatcher dispatcher(event);

            _windowApplicationKeyPressEventInvoked |= dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(TestWindowApplication::OnKeyPressEvent));
            _windowApplicationKeyReleaseEventInvoked |= dispatcher.Dispatch<KeyReleaseEvent>(KMP_BIND(TestWindowApplication::OnKeyReleaseEvent));
            _windowApplicationKeyCharEventInvoked |= dispatcher.Dispatch<KeyCharEvent>(KMP_BIND(TestWindowApplication::OnKeyCharEvent));

            _windowApplicationMouseMoveEventInvoked |= dispatcher.Dispatch<MouseMoveEvent>(KMP_BIND(TestWindowApplication::OnMouseMoveEvent));
            _windowApplicationMouseScrollEventInvoked |= dispatcher.Dispatch<MouseScrollEvent>(KMP_BIND(TestWindowApplication::OnMouseScrollEvent));
            _windowApplicationMouseButtonPressEventInvoked |= dispatcher.Dispatch<MouseButtonPressEvent>(KMP_BIND(TestWindowApplication::OnMouseButtonPressEvent));
            _windowApplicationMouseButtonReleaseEventInvoked |= dispatcher.Dispatch<MouseButtonReleaseEvent>(KMP_BIND(TestWindowApplication::OnMouseButtonReleaseEvent));

            _windowApplicationWindowCloseEventInvoked |= dispatcher.Dispatch<WindowCloseEvent>(KMP_BIND(TestWindowApplication::OnWindowCloseEvent));
            _windowApplicationWindowMoveEventInvoked |= dispatcher.Dispatch<WindowMoveEvent>(KMP_BIND(TestWindowApplication::OnWindowMoveEvent));
            _windowApplicationWindowResizeEventInvoked |= dispatcher.Dispatch<WindowResizeEvent>(KMP_BIND(TestWindowApplication::OnWindowResizeEvent));
            _windowApplicationWindowFocusEventInvoked |= dispatcher.Dispatch<WindowFocusEvent>(KMP_BIND(TestWindowApplication::OnWindowFocusEvent));
            _windowApplicationWindowIconifyEventInvoked |= dispatcher.Dispatch<WindowIconifyEvent>(KMP_BIND(TestWindowApplication::OnWindowIconifyEvent));
            _windowApplicationWindowFramebufferRefreshEventInvoked |= dispatcher.Dispatch<WindowFramebufferRefreshEvent>(KMP_BIND(TestWindowApplication::OnWindowFramebufferRefreshEvent));
            _windowApplicationWindowFramebufferResizeEventInvoked |= dispatcher.Dispatch<WindowFramebufferResizeEvent>(KMP_BIND(TestWindowApplication::OnWindowFramebufferResizeEvent));
        }

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent& evt) override
        {
            if (evt.GetKeyCode() == Key::Enter)
            {
                _completed = true;
            }
            _keyPressEventInvoked = true; 
            return WindowApplication::OnKeyPressEvent(evt);
        }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent& evt) override { _keyReleaseEventInvoked = true; return WindowApplication::OnKeyReleaseEvent(evt); }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent& evt) override { _keyCharEventInvoked = true; return WindowApplication::OnKeyCharEvent(evt); }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent& evt) override { _mouseMoveEventInvoked = true; return WindowApplication::OnMouseMoveEvent(evt); }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent& evt) override { _mouseScrollEventInvoked = true; return WindowApplication::OnMouseScrollEvent(evt); }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& evt) override { _mouseButtonPressEventInvoked = true; return WindowApplication::OnMouseButtonPressEvent(evt); }
        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& evt) override { _mouseButtonReleaseEventInvoked = true; return WindowApplication::OnMouseButtonReleaseEvent(evt); }

        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent& evt)
        {
            _completed = true;
            _windowCloseEventInvoked = true;
            return WindowApplication::OnWindowCloseEvent(evt);
        }
        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent& evt) { _windowMoveEventInvoked = true; return WindowApplication::OnWindowMoveEvent(evt); }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent& evt) { _windowResizeEventInvoked = true; return WindowApplication::OnWindowResizeEvent(evt); }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent& evt) { _windowFocusEventInvoked = true; return WindowApplication::OnWindowFocusEvent(evt); }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent& evt) { _windowIconifyEventInvoked = true; return WindowApplication::OnWindowIconifyEvent(evt); }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& evt) { _windowFramebufferRefreshEventInvoked = true; return WindowApplication::OnWindowFramebufferRefreshEvent(evt); }
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& evt) { _windowFramebufferResizeEventInvoked = true; return WindowApplication::OnWindowFramebufferResizeEvent(evt); }

    private:
        bool _completed = false;

        bool _keyPressEventInvoked = false;
        bool _keyReleaseEventInvoked = false;
        bool _keyCharEventInvoked = false;
        bool _windowApplicationKeyPressEventInvoked = false;
        bool _windowApplicationKeyReleaseEventInvoked = false;
        bool _windowApplicationKeyCharEventInvoked = false;

        bool _mouseMoveEventInvoked = false;
        bool _mouseScrollEventInvoked = false;
        bool _mouseButtonPressEventInvoked = false;
        bool _mouseButtonReleaseEventInvoked = false;
        bool _windowApplicationMouseMoveEventInvoked = false;
        bool _windowApplicationMouseScrollEventInvoked = false;
        bool _windowApplicationMouseButtonPressEventInvoked = false;
        bool _windowApplicationMouseButtonReleaseEventInvoked = false;

        bool _windowCloseEventInvoked = false;
        bool _windowMoveEventInvoked = false;
        bool _windowResizeEventInvoked = false;
        bool _windowFocusEventInvoked = false;
        bool _windowIconifyEventInvoked = false;
        bool _windowFramebufferRefreshEventInvoked = false;
        bool _windowFramebufferResizeEventInvoked = false;
        bool _windowApplicationWindowCloseEventInvoked = false;
        bool _windowApplicationWindowMoveEventInvoked = false;
        bool _windowApplicationWindowResizeEventInvoked = false;
        bool _windowApplicationWindowFocusEventInvoked = false;
        bool _windowApplicationWindowIconifyEventInvoked = false;
        bool _windowApplicationWindowFramebufferRefreshEventInvoked = false;
        bool _windowApplicationWindowFramebufferResizeEventInvoked = false;
    };
}
//--------------------------------------------------------------------------

TEST_CASE("Test window application initialize", "[core][window_application][application][window]")
{
    {
        const auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>("", "Kmplete_window_application_tests_settings.json");

        REQUIRE(application);
        REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());
    }

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_window_application_tests_settings.json");
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(settingsPath));
    REQUIRE(Kmplete::Filesystem::PathExists(settingsPath));
}
//--------------------------------------------------------------------------

TEST_CASE("Test window application key events", "[core][window_application][application][window]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>("", "Kmplete_window_application_tests_settings.json");

    REQUIRE(application);
    REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());

    REQUIRE_FALSE(application->IsKeyPressEventInvoked());
    REQUIRE_FALSE(application->IsKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->IsKeyCharEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationKeyPressEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationKeyCharEventInvoked());

    KMP_MB_UNUSED const auto r = Kmplete::FileDialogs::OpenMessage("Test window application test: KEY", "Press some keys and then press ENTER", Kmplete::FileDialogs::MessageChoice::Ok);
    application->Run();

    REQUIRE(application->IsKeyPressEventInvoked());
    REQUIRE(application->IsKeyReleaseEventInvoked());
    REQUIRE(application->IsKeyCharEventInvoked());
    REQUIRE(application->IsWindowApplicationKeyPressEventInvoked());
    REQUIRE(application->IsWindowApplicationKeyReleaseEventInvoked());
    REQUIRE(application->IsWindowApplicationKeyCharEventInvoked());
}

TEST_CASE("Test window application mouse events", "[core][window_application][application][window]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>("", "Kmplete_window_application_tests_settings.json");

    REQUIRE(application);
    REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());

    REQUIRE_FALSE(application->IsMouseMoveEventInvoked());
    REQUIRE_FALSE(application->IsMouseScrollEventInvoked());
    REQUIRE_FALSE(application->IsMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->IsMouseButtonReleaseEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseMoveEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseScrollEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseButtonReleaseEventInvoked());

    KMP_MB_UNUSED const auto r = Kmplete::FileDialogs::OpenMessage("Test window application test: MOUSE", "Move mouse cursor, scroll, press/release some mouse button and then press ENTER", Kmplete::FileDialogs::MessageChoice::Ok);
    application->Run();

    REQUIRE(application->IsMouseMoveEventInvoked());
    REQUIRE(application->IsMouseScrollEventInvoked());
    REQUIRE(application->IsMouseButtonPressEventInvoked());
    REQUIRE(application->IsMouseButtonReleaseEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseMoveEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseScrollEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseButtonPressEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseButtonReleaseEventInvoked());
}

TEST_CASE("Test window application window events", "[core][window_application][application][window]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>("", "Kmplete_window_application_tests_settings.json");

    REQUIRE(application);
    REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());

    REQUIRE_FALSE(application->IsWindowCloseEventInvoked());
    REQUIRE_FALSE(application->IsWindowMoveEventInvoked());
    //REQUIRE_FALSE(application->IsWindowResizeEventInvoked()); // TODO: window size callback not implemented
    REQUIRE_FALSE(application->IsWindowFocusEventInvoked());
    REQUIRE_FALSE(application->IsWindowIconifyEventInvoked());
    REQUIRE_FALSE(application->IsWindowFramebufferRefreshEventInvoked());
    REQUIRE_FALSE(application->IsWindowFramebufferResizeEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationWindowCloseEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationWindowMoveEventInvoked());
    //REQUIRE_FALSE(application->IsWindowApplicationWindowResizeEventInvoked()); // TODO: window size callback not implemented
    REQUIRE_FALSE(application->IsWindowApplicationWindowFocusEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationWindowIconifyEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationWindowFramebufferRefreshEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationWindowFramebufferResizeEventInvoked());

    KMP_MB_UNUSED const auto r = Kmplete::FileDialogs::OpenMessage("Test window application test: WINDOW", "Move, resize, iconify/deiconify, focus/unfocus window and then close it", Kmplete::FileDialogs::MessageChoice::Ok);
    application->Run();

    REQUIRE(application->IsWindowCloseEventInvoked());
    REQUIRE(application->IsWindowMoveEventInvoked());
    //REQUIRE(application->IsWindowResizeEventInvoked()); // TODO: window size callback not implemented
    REQUIRE(application->IsWindowFocusEventInvoked());
    REQUIRE(application->IsWindowIconifyEventInvoked());
    REQUIRE(application->IsWindowFramebufferRefreshEventInvoked());
    REQUIRE(application->IsWindowFramebufferResizeEventInvoked());
    REQUIRE(application->IsWindowApplicationWindowCloseEventInvoked());
    REQUIRE(application->IsWindowApplicationWindowMoveEventInvoked());
    //REQUIRE(application->IsWindowApplicationWindowResizeEventInvoked()); // TODO: window size callback not implemented
    REQUIRE(application->IsWindowApplicationWindowFocusEventInvoked());
    REQUIRE(application->IsWindowApplicationWindowIconifyEventInvoked());
    REQUIRE(application->IsWindowApplicationWindowFramebufferRefreshEventInvoked());
    REQUIRE(application->IsWindowApplicationWindowFramebufferResizeEventInvoked());
}
//--------------------------------------------------------------------------