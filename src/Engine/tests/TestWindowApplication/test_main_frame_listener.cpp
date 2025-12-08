#include "test_main_frame_listener.h"
#include "test_frame_listeners.h"

#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Assets/font_asset_manager.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Event/event_queue.h"

#include <imgui.h>


namespace Kmplete
{
    static constexpr auto Id_Dockspace = "DockSpace";
    static constexpr auto Id_EventsWindow = "EventsWindow";
    static constexpr auto Id_ControlsWindow = "ControlsWindow";
    static constexpr auto Id_InfoWindow = "InfoWindow";
    static constexpr auto Id_FrameListenersWindow = "FrameListenersInfoWindow";


    TestMainFrameListener::TestMainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, GraphicsBackend* graphicsBackend, WindowBackend* windowBackend, Input::InputManager* inputManager)
        : FrameListener(frameListenerManager, 10ULL, 10)
        , _sharedState(sharedState)
        , _mainWindow(mainWindow)
        , _frameListenerManager(frameListenerManager)
        , _assetsManager(assetsManager)
        , _graphicsBackend(graphicsBackend)
        , _windowBackend(windowBackend)
        , _inputManager(inputManager)
        , _keyPressHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnKeyPressEvent))
        , _keyReleaseHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnKeyReleaseEvent))
        , _keyCharHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnKeyCharEvent))
        , _mouseMoveHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnMouseMoveEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnMouseScrollEvent))
        , _mouseButtonPressedHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnMouseButtonPressEvent))
        , _mouseButtonReleasedHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnMouseButtonReleaseEvent))
        , _windowCloseHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowCloseEvent))
        , _windowMoveHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowMoveEvent))
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowResizeEvent))
        , _windowFocusHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowFocusEvent))
        , _windowIconifyHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowIconifyEvent))
        , _windowFramebufferRefreshHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowFramebufferRefreshEvent))
        , _windowFramebufferResizeHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowFramebufferResizeEvent))
        , _windowContentScaleHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnWindowContentScaleEvent))
        , _customEventHandler(_eventDispatcher, KMP_BIND(TestMainFrameListener::OnCustomEvent))
    {
        Initialize();
    }

    void TestMainFrameListener::Initialize()
    {
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));
        const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale(), 15);
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
                _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale(), 15);
            }
            else
            {
                const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
                _imguiImpl->AddFont(fontPath, _mainWindow.GetDPIScale(), 15);
            }

            io.Fonts->Build();
            _imguiImpl->CreateFontsTexture();

            _switchFontRequested = false;
        }

        _sharedState.updateMaskString += "M";
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

            ImGui::Separator();
            ImGui::Checkbox("CustomEvent", &_customEventInvoked);
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
            auto frame1Exist = bool(_sharedState.existenceMask & frame1Mask);
            auto frame2Exist = bool(_sharedState.existenceMask & frame2Mask);
            auto frame3Exist = bool(_sharedState.existenceMask & frame3Mask);
            auto frame4Exist = bool(_sharedState.existenceMask & frame4Mask);

            auto frame1Active = bool(_sharedState.updateMask & frame1Mask);
            auto frame2Active = bool(_sharedState.updateMask & frame2Mask);
            auto frame3Active = bool(_sharedState.updateMask & frame3Mask);
            auto frame4Active = bool(_sharedState.updateMask & frame4Mask);

            auto frame1AcceptEvent = bool(_sharedState.eventAcceptMask & frame1Mask);
            auto frame2AcceptEvent = bool(_sharedState.eventAcceptMask & frame2Mask);
            auto frame3AcceptEvent = bool(_sharedState.eventAcceptMask & frame3Mask);
            auto frame4AcceptEvent = bool(_sharedState.eventAcceptMask & frame4Mask);


            ImGui::Text("FrameListener 1");
            ImGui::SameLine();
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                ImGui::Checkbox("Exist1", &frame1Exist);
                ImGui::SameLine();
                ImGui::Checkbox("Active1", &frame1Active);
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Accept event1", &frame1AcceptEvent))
            {
                if (frame1AcceptEvent)
                {
                    _sharedState.eventAcceptMask |= frame1Mask;
                }
                else
                {
                    _sharedState.eventAcceptMask &= ~frame1Mask;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete FL1"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Delete, .sid = 1ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Create FL1"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Create, .sid = 1ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Activate FL1"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Activate, .sid = 1ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Deactivate FL1"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Deactivate, .sid = 1ULL });
            }


            ImGui::Text("FrameListener 2");
            ImGui::SameLine();
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                ImGui::Checkbox("Exist2", &frame2Exist);
                ImGui::SameLine();
                ImGui::Checkbox("Active2", &frame2Active);
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Accept event2", &frame2AcceptEvent))
            {
                if (frame2AcceptEvent)
                {
                    _sharedState.eventAcceptMask |= frame2Mask;
                }
                else
                {
                    _sharedState.eventAcceptMask &= ~frame2Mask;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete FL2"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Delete, .sid = 2ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Create FL2"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Create, .sid = 2ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Activate FL2"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Activate, .sid = 2ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Deactivate FL2"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Deactivate, .sid = 2ULL });
            }


            ImGui::Text("FrameListener 3");
            ImGui::SameLine();
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                ImGui::Checkbox("Exist3", &frame3Exist);
                ImGui::SameLine();
                ImGui::Checkbox("Active3", &frame3Active);
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Accept event3", &frame3AcceptEvent))
            {
                if (frame3AcceptEvent)
                {
                    _sharedState.eventAcceptMask |= frame3Mask;
                }
                else
                {
                    _sharedState.eventAcceptMask &= ~frame3Mask;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete FL3"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Delete, .sid = 3ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Create FL3"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Create, .sid = 3ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Activate FL3"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Activate, .sid = 3ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Deactivate FL3"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Deactivate, .sid = 3ULL });
            }


            ImGui::Text("FrameListener 4");
            ImGui::SameLine();
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                ImGui::Checkbox("Exist4", &frame4Exist);
                ImGui::SameLine();
                ImGui::Checkbox("Active4", &frame4Active);
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Accept event4", &frame4AcceptEvent))
            {
                if (frame4AcceptEvent)
                {
                    _sharedState.eventAcceptMask |= frame4Mask;
                }
                else
                {
                    _sharedState.eventAcceptMask &= ~frame4Mask;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete FL4"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Delete, .sid = 4ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Create FL4"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Create, .sid = 4ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Activate FL4"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Activate, .sid = 4ULL });
            }
            ImGui::SameLine();
            if (ImGui::Button("Deactivate FL4"))
            {
                PushCommand(FrameListenerCommand{ .code = FrameListenerCommandCode::Deactivate, .sid = 4ULL });
            }


            ImGui::Text("Update order: %s", _sharedState.updateMaskString.c_str());
            ImGui::SameLine();
            ImGui::Text("OnMouseClickEvent order: %s", _sharedState.eventProcessingString.c_str());

            if (ImGui::Button("Add frame listener with existing SID"))
            {
                _duplicateSidFrameListenerCheckActivated = true;
                try
                {
                    auto newFrameListener = new TestDuplicateSidFrameListener(_frameListenerManager);
                    delete newFrameListener;
                }
                catch (...)
                {
                    _duplicateSidFrameListenerCheckSuccess = true;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Add frame listener with existing priority"))
            {
                _duplicatePriorityFrameListenerCheckActivated = true;
                try
                {
                    auto newFrameListener = new TestDuplicatePriorityFrameListener(_frameListenerManager);
                    delete newFrameListener;
                }
                catch (...)
                {
                    _duplicatePriorityFrameListenerCheckSuccess = true;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Activate mouse button click handler"))
            {
                SetMouseClickHandlerActive(true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Deactivate mouse button click handler"))
            {
                SetMouseClickHandlerActive(false);
            }

            if (ImGui::Button("Trigger custom event"))
            {
                Events::QueueEvent(CreateUPtr<Events::CustomEvent>());
            }
        }
        ImGui::End(); //Id_FrameListenersWindow

        ImGui::End(); //Id_Dockspace

        _imguiImpl->Render();
        ImGui::EndFrame();

        _sharedState.updateMask = 0;
        _sharedState.updateMaskString = "";
        _sharedState.eventProcessingString = "";
    }

    bool TestMainFrameListener::MousePositionIsNotZero() const
    {
        const auto cursorPosition = _mainWindow.GetCursorPosition();
        return cursorPosition.x != 0 || cursorPosition.y != 0;
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

    void TestMainFrameListener::SetMouseClickHandlerActive(bool active)
    {
        if (active)
        {
            _mouseButtonPressedHandler.Attach();
        }
        else
        {
            _mouseButtonPressedHandler.Detach();
        }
    }

    bool TestMainFrameListener::OnMouseButtonPressEvent(Events::MouseButtonPressEvent& evt)
    {
        _mouseButtonPressEventInvoked = true;
        if (evt.GetMouseButton() == Input::Mouse::ButtonLeft && evt.GetMods() & Input::Mode::Ctrl)
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
        else if (evt.GetMouseButton() == Input::Mouse::ButtonRight && evt.GetMods() & Input::Mode::Ctrl)
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

        _sharedState.eventProcessingString += "M";

        return false;
    }

    bool TestMainFrameListener::OnWindowFramebufferRefreshEvent(Events::WindowFramebufferRefreshEvent&)
    {
        _windowFramebufferRefreshEventInvoked = true;
        Render();
        return true;
    }

    bool TestMainFrameListener::OnWindowContentScaleEvent(Events::WindowContentScaleEvent& evt)
    {
        const auto scale = evt.GetScale();

        _imguiImpl.reset();
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));

        const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), scale, 15);
        _imguiImpl->Stylize(scale);

        return true;
    }

    bool TestMainFrameListener::OnWindowCloseEvent(Events::WindowCloseEvent&)
    {
        _mainWindow.SetShouldClose(true);
        _windowCloseEventInvoked = true;
        return true;
    }
}