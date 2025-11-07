#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/scope_guards.h"

#include <catch2/catch_test_macros.hpp>
#include <imgui.h>


namespace Kmplete
{
    class TestWindowApplication : public WindowApplication
    {
    public:
        TestWindowApplication(const WindowApplicationParameters& parameters)
            : WindowApplication(parameters)
        {
            Initialize();
        }

        void Initialize()
        {
            auto& mainWindow = _windowBackend->GetMainWindow();
            mainWindow.SetEventCallback(KMP_BIND(TestWindowApplication::OnEvent));

            _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true));
            _imguiImpl->Stylize(mainWindow.GetDPIScale());

            ImGuiIO& io = ImGui::GetIO();
            io.IniFilename = "imgui_test_app.ini";
        }

        void Run() override
        {
            auto& mainWindow = _windowBackend->GetMainWindow();
            while (!mainWindow.ShouldClose())
            {
                Render();

                mainWindow.ProcessEvents();
                mainWindow.SwapBuffers();
            }
        }

        void SetCustomIconFromFilepath()
        {
            auto& mainWindow = _windowBackend->GetMainWindow();
            const auto iconImage = Image(Filepath(KMP_TEST_ICON_PATH), ImageChannels::RGBAlpha);
            mainWindow.SetIcon(iconImage);
        }

        void SetCustomIconFromBuffer()
        {
            const auto iconBufferSize = 4 * 2 * 4;
            unsigned char iconBuffer[] = {
                /*blue*/ 0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,
                /*red */ 255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255 };
            Image iconFromBuffer(&iconBuffer[0], iconBufferSize, Math::Size2I(4, 2), ImageChannels::RGBAlpha);

            auto& mainWindow = _windowBackend->GetMainWindow();
            mainWindow.SetIcon(iconFromBuffer);
        }

        void SetCustomCursor()
        {
            const auto cursor = _windowBackend->AddCursor("test cursor", Utils::Concatenate(KMP_ICONS_FOLDER, "test_cursor.png"));
            auto& mainWindow = _windowBackend->GetMainWindow();

            if (cursor)
            {
                mainWindow.SetCursor(*cursor);
            }
        }

        void Render()
        {
            _imguiImpl->NewFrame();

            constexpr auto dockFlags =
                ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            {
                ImGuiUtils::StyleVarGuard colorGuard({ {ImGuiStyleVar_WindowRounding, 0.0f}, {ImGuiStyleVar_WindowBorderSize, 0.0f}, {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)} });
                ImGui::Begin("DockSpace_test", nullptr, dockFlags);
            }

            const ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("DockSpace_test");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            }

            static constexpr auto applicationWindowFlags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::Begin("EventsWindow", nullptr, applicationWindowFlags);
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                ImGui::Checkbox("KeyPress", &_keyPressEventInvoked);
                ImGui::Checkbox("KeyRelease", &_keyReleaseEventInvoked);
                ImGui::Checkbox("KeyChar", &_keyCharEventInvoked);

                ImGui::Separator();
                ImGui::Checkbox("MouseMove", &_mouseMoveEventInvoked);
                ImGui::Checkbox("MouseScroll", &_mouseScrollEventInvoked);
                ImGui::Checkbox("MousePress", &_mouseButtonPressEventInvoked);
                ImGui::Checkbox("MouseRelease", &_mouseButtonReleaseEventInvoked);

                ImGui::Separator();
                ImGui::Checkbox("WindowMove", &_windowMoveEventInvoked);
                ImGui::Checkbox("WindowResize", &_windowResizeEventInvoked);
                ImGui::Checkbox("WindowFocus", &_windowFocusEventInvoked);
                ImGui::Checkbox("WindowIconify", &_windowIconifyEventInvoked);
                ImGui::Checkbox("FramebufferRefresh", &_windowFramebufferRefreshEventInvoked);
                ImGui::Checkbox("FramebufferResize", &_windowFramebufferResizeEventInvoked);
            }
            ImGui::End(); //"EventsWindow"

            ImGui::Begin("ControlsWindow", nullptr, applicationWindowFlags);
            {
                if (ImGui::Button("Set icon from filepath"))
                {
                    SetCustomIconFromFilepath();
                }

                if (ImGui::Button("Set icon from buffer"))
                {
                    SetCustomIconFromBuffer();
                }

                if (ImGui::Button("Set custom cursor"))
                {
                    SetCustomCursor();
                }
            }
            ImGui::End(); //"ControlsWindow"

            ImGui::Begin("InfoWindow", nullptr, applicationWindowFlags);
            {
                ImGui::Text("Mouse position: [%d:%d]", _mouseX, _mouseY);
            }
            ImGui::End(); //"InfoWindow"

            ImGui::End(); //"DockSpace_test"

            _imguiImpl->Render();
            ImGui::EndFrame();
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
        void OnEvent(Event& event)
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

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { _keyPressEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { _keyReleaseEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { _keyCharEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent& evt)
        {
            _mouseMoveEventInvoked = true;
            _mouseX = static_cast<int>(evt.GetX());
            _mouseY = static_cast<int>(evt.GetY());

            return true;
        }

        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent&) { _mouseScrollEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& evt)
        {
            _mouseButtonPressEventInvoked = true; 
            auto& mainWindow = _windowBackend->GetMainWindow();
            if (evt.GetMouseButton() == Mouse::ButtonLeft && evt.GetMods() & Mode::Ctrl)
            {
                if (mainWindow.GetCursorMode() == Window::CursorMode::Default)
                {
                    mainWindow.SetCursorMode(Window::CursorMode::Hidden);
                }
                else
                {
                    mainWindow.SetCursorMode(Window::CursorMode::Default);
                }
            }
            else if (evt.GetMouseButton() == Mouse::ButtonRight && evt.GetMods() & Mode::Ctrl)
            {
                if (mainWindow.GetCursorMode() == Window::CursorMode::Default)
                {
                    mainWindow.SetCursorMode(Window::CursorMode::Disabled);
                }
                else
                {
                    mainWindow.SetCursorMode(Window::CursorMode::Default);
                }
            }
            return true;
        }

        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent&) { _mouseButtonReleaseEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent&) { _windowMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent&) { _windowResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent&) { _windowFocusEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent&) { _windowIconifyEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&) { _windowFramebufferRefreshEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent&)
        {
            auto& mainWindow = _windowBackend->GetMainWindow();
            mainWindow.SetShouldClose(true);
            _windowCloseEventInvoked = true;
            return true;
        }

    private:
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;

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

        int _mouseX = 0;
        int _mouseY = 0;
    };
}

TEST_CASE("Test window application", "[window_application][application][window][event]")
{
    auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>(Kmplete::WindowApplicationParameters{ .applicationParameters{"TestApplication", "", KMP_TEST_SETTINGS_JSON}, .resizable = true });

    REQUIRE(application);
    REQUIRE(!Kmplete::Filesystem::GetCurrentFilepath().empty());

    REQUIRE_FALSE(application->IsKeyPressEventInvoked());
    REQUIRE_FALSE(application->IsKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->IsKeyCharEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationKeyPressEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationKeyCharEventInvoked());

    REQUIRE_FALSE(application->IsMouseMoveEventInvoked());
    REQUIRE_FALSE(application->IsMouseScrollEventInvoked());
    REQUIRE_FALSE(application->IsMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->IsMouseButtonReleaseEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseMoveEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseScrollEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->IsWindowApplicationMouseButtonReleaseEventInvoked());

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

    application->Run();

    REQUIRE(application->IsKeyPressEventInvoked());
    REQUIRE(application->IsKeyReleaseEventInvoked());
    REQUIRE(application->IsKeyCharEventInvoked());
    REQUIRE(application->IsWindowApplicationKeyPressEventInvoked());
    REQUIRE(application->IsWindowApplicationKeyReleaseEventInvoked());
    REQUIRE(application->IsWindowApplicationKeyCharEventInvoked());

    REQUIRE(application->IsMouseMoveEventInvoked());
    REQUIRE(application->IsMouseScrollEventInvoked());
    REQUIRE(application->IsMouseButtonPressEventInvoked());
    REQUIRE(application->IsMouseButtonReleaseEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseMoveEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseScrollEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseButtonPressEventInvoked());
    REQUIRE(application->IsWindowApplicationMouseButtonReleaseEventInvoked());

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

    application.reset();

    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append(KMP_TEST_SETTINGS_JSON);
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(settingsFilepath));
    REQUIRE(Kmplete::Filesystem::FilepathExists(settingsFilepath));
}
