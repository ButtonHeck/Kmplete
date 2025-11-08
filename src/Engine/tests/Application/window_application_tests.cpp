#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Graphics/font_manager.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/scope_guards.h"

#include <catch2/catch_test_macros.hpp>
#include <imgui.h>


namespace Kmplete
{
    static constexpr auto Id_Dockspace = "DockSpace";
    static constexpr auto Id_EventsWindow = "EventsWindow";
    static constexpr auto Id_ControlsWindow = "ControlsWindow";
    static constexpr auto Id_InfoWindow = "InfoWindow";

    class TestWindowApplication : public WindowApplication
    {
    public:
        TestWindowApplication(const WindowApplicationParameters& parameters)
            : WindowApplication(parameters)
            , _mainWindow(_windowBackend->GetMainWindow())
        {
            Initialize();
        }

        void Initialize()
        {
            _mainWindow.SetEventCallback(KMP_BIND(TestWindowApplication::OnEvent));
            const auto scale = _mainWindow.GetDPIScale();

            _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));
            const auto& defaultFont = _assetsManager->GetFontManager().GetFont(FontManager::DefaultFontSID);
            _imguiImpl->AddFont(defaultFont.GetBuffer(), scale);
            _imguiImpl->Stylize(scale);
        }

        void Run() override
        {
            while (!_mainWindow.ShouldClose())
            {
                Render();

                _mainWindow.ProcessEvents();
                _mainWindow.SwapBuffers();
            }
        }

        void SetCustomIconFromFilepath()
        {
            const auto iconImage = Image(Filepath(KMP_TEST_ICON_PATH), ImageChannels::RGBAlpha);
            _mainWindow.SetIcon(iconImage);
        }

        void SetCustomIconFromBuffer()
        {
            const auto iconBufferSize = 4 * 2 * 4;
            unsigned char iconBuffer[] = {
                /*blue*/ 0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,
                /*red */ 255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255 };
            Image iconFromBuffer(&iconBuffer[0], iconBufferSize, Math::Size2I(4, 2), ImageChannels::RGBAlpha);

            _mainWindow.SetIcon(iconFromBuffer);
        }

        void SetCustomCursor()
        {
            const auto cursor = _windowBackend->AddCursor("test cursor", Utils::Concatenate(KMP_ICONS_FOLDER, "test_cursor.png"));

            if (cursor)
            {
                _mainWindow.SetCursor(*cursor);
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
                ImGui::Begin(Id_Dockspace, nullptr, dockFlags);
            }

            const ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID(Id_Dockspace);
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            }

            static constexpr auto applicationWindowFlags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::Begin(Id_EventsWindow, nullptr, applicationWindowFlags);
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
            ImGui::End(); //Id_EventsWindow

            ImGui::Begin(Id_ControlsWindow, nullptr, applicationWindowFlags);
            {
                auto updateContinuously = _mainWindow.IsUpdatedContinuously();
                if (ImGui::Checkbox("Update continuously", &updateContinuously))
                {
                    _mainWindow.SetUpdatedContinuously(updateContinuously);
                }

                auto vSync = _mainWindow.IsVSync();
                if (ImGui::Checkbox("VSync", &vSync))
                {
                    _mainWindow.SetVSync(vSync);
                }

                auto alwaysOnTop = _mainWindow.IsAlwaysOnTop();
                if (ImGui::Checkbox("Always on top", &alwaysOnTop))
                {
                    _mainWindow.SetAlwaysOnTop(alwaysOnTop);
                }

                auto decorated = _mainWindow.IsDecorated();
                if (ImGui::Checkbox("Decorated", &decorated))
                {
                    _mainWindow.SetDecorated(decorated);
                }

                auto resizable = _mainWindow.IsResizable();
                if (ImGui::Checkbox("Resizable", &resizable))
                {
                    _mainWindow.SetResizable(resizable);
                }

                const auto screenMode = _mainWindow.GetScreenMode();
                const auto screenModeStr = Window::ScreenModeToString(_mainWindow.GetScreenMode());
                if (ImGui::BeginCombo("Screen mode", screenModeStr.c_str()))
                {
                    if (ImGui::Selectable("Windowed", screenMode == Window::ScreenMode::Windowed))
                    {
                        _mainWindow.SetScreenMode(Window::ScreenMode::Windowed);
                    }
                    if (ImGui::Selectable("Fullscreen", screenMode == Window::ScreenMode::Fullscreen))
                    {
                        _mainWindow.SetScreenMode(Window::ScreenMode::Fullscreen);
                    }
                    if (ImGui::Selectable("Windowed Fullscreen", screenMode == Window::ScreenMode::WindowedFullscreen))
                    {
                        _mainWindow.SetScreenMode(Window::ScreenMode::WindowedFullscreen);
                    }
                    ImGui::EndCombo();
                }

                ImGui::Separator();

                if (ImGui::Button("SetPosition 50x50"))
                {
                    _mainWindow.SetPosition(50, 50);
                }

                if (ImGui::Button("Position at screen's center"))
                {
                    _mainWindow.PositionAtCurrentScreenCenter();
                }

                if (ImGui::Button("Set custom title"))
                {
                    _mainWindow.SetTitle("Custom title successfully set!");
                }

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
            ImGui::End(); //Id_ControlsWindow

            ImGui::Begin(Id_InfoWindow, nullptr, applicationWindowFlags);
            {
                ImGuiStyle& style = ImGui::GetStyle();
                const auto oldTextColor = style.Colors[ImGuiCol_Text];
                style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                ImGui::Text("Press Ctrl+LMB/RMB to changed cursor mode");
                style.Colors[ImGuiCol_Text] = oldTextColor;


                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);


                const auto windowPos = _mainWindow.GetPosition();
                ImGui::Text("Window position: [%d:%d]", windowPos.x, windowPos.y);
                ImGui::SameLine(0.0f, 32.0f);

                const auto windowSize = _mainWindow.GetSize();
                ImGui::Text("Window size: [%dx%d]", windowSize.x, windowSize.y);


                const auto dpi = _mainWindow.GetDPI();
                ImGui::Text("DPI: %d", dpi);
                ImGui::SameLine(0.0f, 32.0f);

                const auto dpiScale = _mainWindow.GetDPIScale();
                ImGui::Text("DPI scale: %.2f", dpiScale);
                ImGui::SameLine(0.0f, 32.0f);

                const auto screenModeStr = Window::ScreenModeToString(_mainWindow.GetScreenMode());
                ImGui::Text("Screen mode: %s", screenModeStr.c_str());
                

                const auto cursorPosition = _mainWindow.GetCursorPosition();
                ImGui::Text("Mouse position: [%d:%d]", cursorPosition.x, cursorPosition.y);
                ImGui::SameLine(0.0f, 32.0f);

                const auto cursorMode = _mainWindow.GetCursorMode();
                if (cursorMode == Window::CursorMode::Default)
                {
                    ImGui::Text("Cursor mode: Default");
                }
                else if (cursorMode == Window::CursorMode::Hidden)
                {
                    ImGui::Text("Cursor mode: Hidden");
                }
                else
                {
                    ImGui::Text("Cursor mode: Disabled");
                }
            }
            ImGui::End(); //Id_InfoWindow

            ImGui::End(); //Id_Dockspace

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

        bool MousePositionIsNotZero() const
        {
            const auto cursorPosition = _mainWindow.GetCursorPosition();
            return cursorPosition.x != 0 && cursorPosition.y != 0;
        }

        bool DPIIsNotZero() const
        {
            return _mainWindow.GetDPI() > 0;
        }

        bool DPIScaleIsNotZero() const
        {
            return _mainWindow.GetDPIScale() > 0.0f;
        }

        bool DefaultSizeIsNotZero() const
        {
            const auto size = _mainWindow.GetSize();
            return size.x > 0 && size.y > 0;
        }

        bool DefaultWindowedSizeIsNotZero() const
        {
            const auto windowedSize = _mainWindow.GetWindowedSize();
            return windowedSize.x > 0 && windowedSize.y > 0;
        }

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

            dispatcher.Dispatch<WindowContentScaleEvent>(KMP_BIND(TestWindowApplication::OnWindowContentScaleEvent));
        }

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { _keyPressEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { _keyReleaseEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { _keyCharEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent&) { _mouseMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent&) { _mouseScrollEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& evt)
        {
            _mouseButtonPressEventInvoked = true; 
            if (evt.GetMouseButton() == Mouse::ButtonLeft && evt.GetMods() & Mode::Ctrl)
            {
                if (_mainWindow.GetCursorMode() == Window::CursorMode::Default)
                {
                    _mainWindow.SetCursorMode(Window::CursorMode::Hidden);
                }
                else
                {
                    _mainWindow.SetCursorMode(Window::CursorMode::Default);
                }
            }
            else if (evt.GetMouseButton() == Mouse::ButtonRight && evt.GetMods() & Mode::Ctrl)
            {
                if (_mainWindow.GetCursorMode() == Window::CursorMode::Default)
                {
                    _mainWindow.SetCursorMode(Window::CursorMode::Disabled);
                }
                else
                {
                    _mainWindow.SetCursorMode(Window::CursorMode::Default);
                }
            }
            return true;
        }

        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent&) { _mouseButtonReleaseEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent&) { _windowMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent&) { _windowResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent&) { _windowFocusEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent&) { _windowIconifyEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
        {
            _windowFramebufferRefreshEventInvoked = true;
            Render();
            return true;
        }

        KMP_NODISCARD virtual bool OnWindowContentScaleEvent(WindowContentScaleEvent& evt)
        {
            const auto scale = evt.GetScale();

            _imguiImpl.reset();
            _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));

            const auto& defaultFont = _assetsManager->GetFontManager().GetFont(FontManager::DefaultFontSID);
            _imguiImpl->AddFont(defaultFont.GetBuffer(), scale);
            _imguiImpl->Stylize(scale);

            return true;
        }

        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent&)
        {
            _mainWindow.SetShouldClose(true);
            _windowCloseEventInvoked = true;
            return true;
        }

    private:
        Window& _mainWindow;
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

    REQUIRE(application->DefaultSizeIsNotZero());
    REQUIRE(application->DefaultWindowedSizeIsNotZero());

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

    REQUIRE(application->MousePositionIsNotZero());
    REQUIRE(application->DPIIsNotZero());
    REQUIRE(application->DPIScaleIsNotZero());

    application.reset();

    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append(KMP_TEST_SETTINGS_JSON);
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(settingsFilepath));
    REQUIRE(Kmplete::Filesystem::FilepathExists(settingsFilepath));
}
