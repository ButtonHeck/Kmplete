#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Assets/font_asset_manager.h"
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
    static constexpr auto Id_FrameListenersWindow = "FrameListenersInfoWindow";

    static constexpr auto frame1Mask = 0b0001;
    static constexpr auto frame2Mask = 0b0010;
    static constexpr auto frame3Mask = 0b0100;
    static constexpr auto frame4Mask = 0b1000;

    
    struct SharedState
    {
        int existenceMask = 0;
        int updateMask = 0;
    };


    class TestFrameListener1 : public FrameListener
    {
    public:
        TestFrameListener1(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 1ULL, 1)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame1Mask;
        }

        ~TestFrameListener1()
        {
            sharedState.existenceMask &= ~frame1Mask;
        }

        SharedState& sharedState;
        const String name = "TestFrameListener1";
    };

    class TestFrameListener2 : public FrameListener
    {
    public:
        TestFrameListener2(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 2ULL, 2)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame2Mask;
        }

        ~TestFrameListener2()
        {
            sharedState.existenceMask &= ~frame2Mask;
        }

        SharedState& sharedState;
        const String name = "TestFrameListener2";
    };

    class TestFrameListener3 : public FrameListener
    {
    public:
        TestFrameListener3(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 3ULL, 3)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame3Mask;
        }

        ~TestFrameListener3()
        {
            sharedState.existenceMask &= ~frame3Mask;
        }

        SharedState& sharedState;
        const String name = "TestFrameListener3";
    };

    class TestFrameListener4 : public FrameListener
    {
    public:
        TestFrameListener4(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 4ULL, 4)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame4Mask;
        }

        ~TestFrameListener4()
        {
            sharedState.existenceMask &= ~frame4Mask;
        }

        SharedState& sharedState;
        const String name = "TestFrameListener4";
    };


    class TestMainFrameListener : public FrameListener
    {
    public:
        TestMainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, GraphicsBackend* graphicsBackend, WindowBackend* windowBackend);

        void Initialize();

        void Update(float /*frameTimestep*/, bool /*applicationIsIconified*/) override;
        void Render() override;

        void SetCustomIconFromFilepath();
        void SetCustomIconFromBuffer();
        void SetCustomCursor();
        void SwitchFonts();

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

        bool MousePositionIsNotZero() const;
        bool DPIIsNotZero() const;
        bool DPIScaleIsNotZero() const;
        bool DefaultSizeIsNotZero() const;
        bool DefaultWindowedSizeIsNotZero() const;

    protected:
        void OnEvent(Event& event) override;

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { _keyPressEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { _keyReleaseEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { _keyCharEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnMouseMoveEvent(MouseMoveEvent&) { _mouseMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseScrollEvent(MouseScrollEvent&) { _mouseScrollEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& evt);

        KMP_NODISCARD virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent&) { _mouseButtonReleaseEventInvoked = true; return true; }

        KMP_NODISCARD virtual bool OnWindowMoveEvent(WindowMoveEvent&) { _windowMoveEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowResizeEvent(WindowResizeEvent&) { _windowResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFocusEvent(WindowFocusEvent&) { _windowFocusEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowIconifyEvent(WindowIconifyEvent&) { _windowIconifyEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&);
        KMP_NODISCARD virtual bool OnWindowContentScaleEvent(WindowContentScaleEvent& evt);
        KMP_NODISCARD virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent&) { _windowFramebufferResizeEventInvoked = true; return true; }
        KMP_NODISCARD virtual bool OnWindowCloseEvent(WindowCloseEvent&);

    private:
        SharedState& _sharedState;
        Window& _mainWindow;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        Assets::AssetsManager* _assetsManager;
        GraphicsBackend* _graphicsBackend;
        WindowBackend* _windowBackend;

        bool _switchFontRequested = false;
        bool _useDefaultFont = true;

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
    

    class TestWindowApplication : public WindowApplication
    {
    public:
        TestWindowApplication(const WindowApplicationParameters& parameters);

        void Initialize();
        void FrameListenerCommandBufferHandlerFunction(const FrameListenerCommandBuffer& commandBuffer);

        // public for simplicity
    public:
        UPtr<TestMainFrameListener> mainFrameListener;
        UPtr<TestFrameListener1> frameListener1;
        UPtr<TestFrameListener2> frameListener2;
        UPtr<TestFrameListener3> frameListener3;
        UPtr<TestFrameListener4> frameListener4;

    private:
        Window& _mainWindow;
        SharedState _sharedState;
    };


    TestMainFrameListener::TestMainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, GraphicsBackend* graphicsBackend, WindowBackend* windowBackend)
        : FrameListener(frameListenerManager, "TestMainFrameListener"_sid, 10)
        , _sharedState(sharedState)
        , _mainWindow(mainWindow)
        , _assetsManager(assetsManager)
        , _graphicsBackend(graphicsBackend)
        , _windowBackend(windowBackend)
    {
        Initialize();
    }

    void TestMainFrameListener::Initialize()
    {
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));
        const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale());
        _imguiImpl->Stylize(_mainWindow.GetDPIScale());
    }

    void TestMainFrameListener::Update(float /*frameTimestep*/, bool /*applicationIsIconified*/)
    {
        if (_switchFontRequested)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->Clear();

            if (_useDefaultFont)
            {
                const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
                _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale());
            }
            else
            {
                const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
                _imguiImpl->AddFont(fontPath, _mainWindow.GetDPIScale());
            }

            io.Fonts->Build();
            _imguiImpl->CreateFontsTexture();

            _switchFontRequested = false;
        }
    }

    void TestMainFrameListener::SetCustomIconFromFilepath()
    {
        const auto iconImage = Image(Filepath(KMP_TEST_ICON_PATH), ImageChannels::RGBAlpha);
        _mainWindow.SetIcon(iconImage);
    }

    void TestMainFrameListener::SetCustomIconFromBuffer()
    {
        const auto iconBufferSize = 4 * 2 * 4;
        unsigned char iconBuffer[] = {
            /*blue*/ 0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,
            /*red */ 255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255 };
        Image iconFromBuffer(&iconBuffer[0], iconBufferSize, Math::Size2I(4, 2), ImageChannels::RGBAlpha);

        _mainWindow.SetIcon(iconFromBuffer);
    }

    void TestMainFrameListener::SetCustomCursor()
    {
        const auto cursor = _windowBackend->AddCursor("test cursor", Utils::Concatenate(KMP_ICONS_FOLDER, "test_cursor.png"));

        if (cursor)
        {
            _mainWindow.SetCursor(*cursor);
        }
    }

    void TestMainFrameListener::SwitchFonts()
    {
        _switchFontRequested = true;
        _useDefaultFont = !_useDefaultFont;
    }

    void TestMainFrameListener::Render()
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

            if (ImGui::Button("Switch fonts"))
            {
                SwitchFonts();
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

        ImGui::Begin(Id_FrameListenersWindow, nullptr, applicationWindowFlags);
        {
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                auto frame1Exist = bool(_sharedState.existenceMask & frame1Mask);
                auto frame2Exist = bool(_sharedState.existenceMask & frame2Mask);
                auto frame3Exist = bool(_sharedState.existenceMask & frame3Mask);
                auto frame4Exist = bool(_sharedState.existenceMask & frame4Mask);

                ImGui::Checkbox("FrameListener1 exist", &frame1Exist);
                ImGui::Checkbox("FrameListener2 exist", &frame2Exist);
                ImGui::Checkbox("FrameListener3 exist", &frame3Exist);
                ImGui::Checkbox("FrameListener4 exist", &frame4Exist);
            }

            if (ImGui::Button("Delete FL1"))
            {
                PushCommand(FrameListenerCommand{ .command = FrameListenerCommandCode::Delete, .frameListenerSidDestination = 1ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Create FL1"))
            {
                PushCommand(FrameListenerCommand{ .command = FrameListenerCommandCode::AddAfter, .frameListenerSidDestination = 1ULL });
            }
        }
        ImGui::End(); //Id_FrameListenersWindow

        ImGui::End(); //Id_Dockspace

        _imguiImpl->Render();
        ImGui::EndFrame();
    }

    bool TestMainFrameListener::MousePositionIsNotZero() const
    {
        const auto cursorPosition = _mainWindow.GetCursorPosition();
        return cursorPosition.x != 0 && cursorPosition.y != 0;
    }

    bool TestMainFrameListener::DPIIsNotZero() const
    {
        return _mainWindow.GetDPI() > 0;
    }

    bool TestMainFrameListener::DPIScaleIsNotZero() const
    {
        return _mainWindow.GetDPIScale() > 0.0f;
    }

    bool TestMainFrameListener::DefaultSizeIsNotZero() const
    {
        const auto size = _mainWindow.GetSize();
        return size.x > 0 && size.y > 0;
    }

    bool TestMainFrameListener::DefaultWindowedSizeIsNotZero() const
    {
        const auto windowedSize = _mainWindow.GetWindowedSize();
        return windowedSize.x > 0 && windowedSize.y > 0;
    }

    void TestMainFrameListener::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        _windowApplicationKeyPressEventInvoked |= dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(TestMainFrameListener::OnKeyPressEvent));
        _windowApplicationKeyReleaseEventInvoked |= dispatcher.Dispatch<KeyReleaseEvent>(KMP_BIND(TestMainFrameListener::OnKeyReleaseEvent));
        _windowApplicationKeyCharEventInvoked |= dispatcher.Dispatch<KeyCharEvent>(KMP_BIND(TestMainFrameListener::OnKeyCharEvent));

        _windowApplicationMouseMoveEventInvoked |= dispatcher.Dispatch<MouseMoveEvent>(KMP_BIND(TestMainFrameListener::OnMouseMoveEvent));
        _windowApplicationMouseScrollEventInvoked |= dispatcher.Dispatch<MouseScrollEvent>(KMP_BIND(TestMainFrameListener::OnMouseScrollEvent));
        _windowApplicationMouseButtonPressEventInvoked |= dispatcher.Dispatch<MouseButtonPressEvent>(KMP_BIND(TestMainFrameListener::OnMouseButtonPressEvent));
        _windowApplicationMouseButtonReleaseEventInvoked |= dispatcher.Dispatch<MouseButtonReleaseEvent>(KMP_BIND(TestMainFrameListener::OnMouseButtonReleaseEvent));

        _windowApplicationWindowCloseEventInvoked |= dispatcher.Dispatch<WindowCloseEvent>(KMP_BIND(TestMainFrameListener::OnWindowCloseEvent));
        _windowApplicationWindowMoveEventInvoked |= dispatcher.Dispatch<WindowMoveEvent>(KMP_BIND(TestMainFrameListener::OnWindowMoveEvent));
        _windowApplicationWindowResizeEventInvoked |= dispatcher.Dispatch<WindowResizeEvent>(KMP_BIND(TestMainFrameListener::OnWindowResizeEvent));
        _windowApplicationWindowFocusEventInvoked |= dispatcher.Dispatch<WindowFocusEvent>(KMP_BIND(TestMainFrameListener::OnWindowFocusEvent));
        _windowApplicationWindowIconifyEventInvoked |= dispatcher.Dispatch<WindowIconifyEvent>(KMP_BIND(TestMainFrameListener::OnWindowIconifyEvent));
        _windowApplicationWindowFramebufferRefreshEventInvoked |= dispatcher.Dispatch<WindowFramebufferRefreshEvent>(KMP_BIND(TestMainFrameListener::OnWindowFramebufferRefreshEvent));
        _windowApplicationWindowFramebufferResizeEventInvoked |= dispatcher.Dispatch<WindowFramebufferResizeEvent>(KMP_BIND(TestMainFrameListener::OnWindowFramebufferResizeEvent));

        dispatcher.Dispatch<WindowContentScaleEvent>(KMP_BIND(TestMainFrameListener::OnWindowContentScaleEvent));
    }

    bool TestMainFrameListener::OnMouseButtonPressEvent(MouseButtonPressEvent& evt)
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

    bool TestMainFrameListener::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        _windowFramebufferRefreshEventInvoked = true;
        Render();
        return true;
    }

    bool TestMainFrameListener::OnWindowContentScaleEvent(WindowContentScaleEvent& evt)
    {
        const auto scale = evt.GetScale();

        _imguiImpl.reset();
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));

        const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), scale);
        _imguiImpl->Stylize(scale);

        return true;
    }

    bool TestMainFrameListener::OnWindowCloseEvent(WindowCloseEvent&)
    {
        _mainWindow.SetShouldClose(true);
        _windowCloseEventInvoked = true;
        return true;
    }


    TestWindowApplication::TestWindowApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , mainFrameListener(nullptr)
        , frameListener1(nullptr)
        , frameListener2(nullptr)
        , frameListener3(nullptr)
        , frameListener4(nullptr)
    {
        Initialize();
    }

    void TestWindowApplication::Initialize()
    {
        frameListener1.reset(new TestFrameListener1(*_frameListenerManager.get(), _sharedState));
        frameListener2.reset(new TestFrameListener2(*_frameListenerManager.get(), _sharedState));
        frameListener3.reset(new TestFrameListener3(*_frameListenerManager.get(), _sharedState));
        frameListener4.reset(new TestFrameListener4(*_frameListenerManager.get(), _sharedState));
        mainFrameListener.reset(new TestMainFrameListener(*_frameListenerManager.get(), _sharedState, _mainWindow, _assetsManager.get(), _graphicsBackend.get(), _windowBackend.get()));

        _frameListenerManager->SetCommandBufferHandler(KMP_BIND(TestWindowApplication::FrameListenerCommandBufferHandlerFunction));
    }

    void TestWindowApplication::FrameListenerCommandBufferHandlerFunction(const FrameListenerCommandBuffer& commandBuffer)
    {
        for (const auto& command : commandBuffer)
        {
            if (command.command == FrameListenerCommandCode::Delete && command.frameListenerSidDestination == 1ULL)
            {
                frameListener1.reset(nullptr);
            }
        }
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Test window application", "[window_application][application][window][event]")
{
    auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>(Kmplete::WindowApplicationParameters{ .applicationParameters{"TestApplication", "", KMP_TEST_SETTINGS_JSON}, .resizable = true });

    REQUIRE(application);
    REQUIRE(!Kmplete::Filesystem::GetCurrentFilepath().empty());

    REQUIRE_FALSE(application->mainFrameListener->IsKeyPressEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsKeyCharEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationKeyPressEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationKeyCharEventInvoked());

    REQUIRE_FALSE(application->mainFrameListener->IsMouseMoveEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsMouseScrollEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsMouseButtonReleaseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationMouseMoveEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationMouseScrollEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationMouseButtonReleaseEventInvoked());

    REQUIRE_FALSE(application->mainFrameListener->IsWindowCloseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowMoveEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowResizeEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowFocusEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowIconifyEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowFramebufferRefreshEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowFramebufferResizeEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowCloseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowMoveEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowResizeEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowFocusEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowIconifyEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowFramebufferRefreshEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowApplicationWindowFramebufferResizeEventInvoked());

    REQUIRE(application->mainFrameListener->DefaultSizeIsNotZero());
    REQUIRE(application->mainFrameListener->DefaultWindowedSizeIsNotZero());

    application->Run();

    REQUIRE(application->mainFrameListener->IsKeyPressEventInvoked());
    REQUIRE(application->mainFrameListener->IsKeyReleaseEventInvoked());
    REQUIRE(application->mainFrameListener->IsKeyCharEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationKeyPressEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationKeyReleaseEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationKeyCharEventInvoked());

    REQUIRE(application->mainFrameListener->IsMouseMoveEventInvoked());
    REQUIRE(application->mainFrameListener->IsMouseScrollEventInvoked());
    REQUIRE(application->mainFrameListener->IsMouseButtonPressEventInvoked());
    REQUIRE(application->mainFrameListener->IsMouseButtonReleaseEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationMouseMoveEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationMouseScrollEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationMouseButtonPressEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationMouseButtonReleaseEventInvoked());

    REQUIRE(application->mainFrameListener->IsWindowCloseEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowMoveEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowResizeEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowFocusEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowIconifyEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowFramebufferRefreshEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowFramebufferResizeEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowCloseEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowMoveEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowResizeEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowFocusEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowIconifyEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowFramebufferRefreshEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowApplicationWindowFramebufferResizeEventInvoked());

    REQUIRE(application->mainFrameListener->MousePositionIsNotZero());
    REQUIRE(application->mainFrameListener->DPIIsNotZero());
    REQUIRE(application->mainFrameListener->DPIScaleIsNotZero());

    application.reset();

    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append(KMP_TEST_SETTINGS_JSON);
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(settingsFilepath));
    REQUIRE(Kmplete::Filesystem::FilepathExists(settingsFilepath));
}
//--------------------------------------------------------------------------
