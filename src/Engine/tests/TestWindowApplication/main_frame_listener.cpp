#include "main_frame_listener.h"
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
    static constexpr auto Id_ActionEventsEmulatorWindow = "ActionEventsEmulatorWindow";


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, SharedState& sharedState, Window& mainWindow, Assets::AssetsManager* assetsManager, 
                                         GraphicsBackend* graphicsBackend, WindowBackend* windowBackend, Input::InputManager* inputManager)
        : FrameListener(frameListenerManager, 10ULL, 10)
        , _sharedState(sharedState)
        , _mainWindow(mainWindow)
        , _frameListenerManager(frameListenerManager)
        , _assetsManager(assetsManager)
        , _graphicsBackend(graphicsBackend)
        , _windowBackend(windowBackend)
        , _inputManager(inputManager)

        , _keyPressHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnKeyPressEvent))
        , _keyReleaseHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnKeyReleaseEvent))
        , _keyCharHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnKeyCharEvent))
        , _mouseMoveHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnMouseMoveEvent))
        , _mouseScrollHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnMouseScrollEvent))
        , _mouseButtonPressedHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnMouseButtonPressEvent))
        , _mouseButtonReleasedHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnMouseButtonReleaseEvent))
        , _windowCloseHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowCloseEvent))
        , _windowMoveHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowMoveEvent))
        , _windowResizeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowResizeEvent))
        , _windowFocusHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowFocusEvent))
        , _windowIconifyHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowIconifyEvent))
        , _windowFramebufferRefreshHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowFramebufferRefreshEvent))
        , _windowFramebufferResizeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowFramebufferResizeEvent))
        , _windowContentScaleHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowContentScaleEvent))
        , _customEventHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnCustomEvent))
    {
        Initialize();
    }

    void MainFrameListener::Initialize()
    {
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));
        const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), _mainWindow.GetDPIScale(), 15);
        _imguiImpl->Stylize(_mainWindow.GetDPIScale());

        _inputManager->MapInputToAction({Input::Code::Key_W, Input::PressNoModsCondition}, "move_forward"_sid);
        _inputManager->MapInputToAction({Input::Code::Key_S, Input::PressNoModsCondition}, "move_backward"_sid);
        _inputManager->MapInputToAction({Input::Code::Key_A, Input::PressNoModsCondition}, "move_left"_sid);
        _inputManager->MapInputToAction({Input::Code::Key_D, Input::PressNoModsCondition}, "move_right"_sid);
        _inputManager->MapInputToCallback({Input::Code::Key_LeftControl, Input::PressNoModsCondition}, "crouch"_sid, [this](Input::InputControlValue value) {
            if (value == Input::ButtonPressedValue)
            {
                _emulatorPlayerCrouching = !_emulatorPlayerCrouching;
            }
            return true;
        });
        _inputManager->MapInputToAction(Input::Code::Mouse_Position, "mouse_tracking"_sid);
        _inputManager->MapInputToAction(Input::Code::Mouse_Move, "mouse_move_tracking"_sid);
        _inputManager->MapActionToCallback("mouse_tracking"_sid, [this](Input::InputControlValue value) {
            _emulatorMousePosCb = std::get<Math::Point2I>(value);
            return true;
        });
        _inputManager->MapActionToCallback("mouse_move_tracking"_sid, [this](Input::InputControlValue value) {
            _emulatorMouseMoveCb = std::get<Math::Point2I>(value);
            return true;
        });

        _actionDefaultTagCallbackDoubleRegistrationCheck = _inputManager->MapActionToCallback("duplicate_default_tag_check"_sid, [](Input::InputControlValue){ return true; });
        _actionDefaultTagCallbackDoubleRegistrationCheck = _inputManager->MapActionToCallback("duplicate_default_tag_check"_sid, [](Input::InputControlValue){ return true; });
        _actionNonDefaultTagCallbackDoubleRegistrationCheck = _inputManager->MapActionToCallback("duplicate_tag_check"_sid, Input::TaggedActionCallback{.tag = "bzz"_sid, .callback = [](Input::InputControlValue) { return true; } });
        _actionNonDefaultTagCallbackDoubleRegistrationCheck = _inputManager->MapActionToCallback("duplicate_tag_check"_sid, Input::TaggedActionCallback{.tag = "bzz"_sid, .callback = [](Input::InputControlValue) { return true; } });
        _unmapUnregisteredActionCheck = _inputManager->UnmapActionFromCallback("unregistered_action"_sid, "bzz"_sid);
        _inputToActionDoubleRegistrationCheck = _inputManager->MapInputToAction({Input::Code::Key_W, Input::PressNoModsCondition}, "move_forward"_sid);
        _unmapInvalidInputFromActionCheck |= _inputManager->UnmapInputFromAction(Input::Code::Key_M, "bzz"_sid);
        _unmapInvalidInputFromActionCheck |= _inputManager->UnmapInputFromAction(Input::Code::Key_M, "move_forward"_sid);
        _remapInvalidInputToActionCheck |= _inputManager->RemapInputToAction({Input::Code::Key_W, Input::PressNoModsCondition}, "bzz"_sid);
    }

    void MainFrameListener::Update(float /*frameTimestep*/, bool /*applicationIsIconified*/)
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

        if (_inputManager->GetActionValue<int>("move_forward"_sid) == Input::ButtonPressed)
        {
            _emulatorPlayerPos.y++;
        }
        if (_inputManager->GetActionValue<int>("move_backward"_sid) == Input::ButtonPressed)
        {
            _emulatorPlayerPos.y--;
        }
        if (_inputManager->GetActionValue<int>("move_left"_sid) == Input::ButtonPressed)
        {
            _emulatorPlayerPos.x--;
        }
        if (_inputManager->GetActionValue<int>("move_right"_sid) == Input::ButtonPressed)
        {
            _emulatorPlayerPos.x++;
        }

        _emulatorMousePosGet = _inputManager->GetActionValue<Math::Point2I>("mouse_tracking"_sid);
        _emulatorMouseMoveGet = _inputManager->GetActionValue<Math::Point2I>("mouse_move_tracking"_sid);
    }

    void MainFrameListener::SetCustomIconFromFilepath()
    {
        const auto iconImage = Image(Filepath(KMP_TEST_ICON_PATH), ImageChannels::RGBAlpha);
        _mainWindow.SetIcon(iconImage);
    }

    void MainFrameListener::SetCustomIconFromBuffer()
    {
        const auto iconBufferSize = 4 * 2 * 4;
        unsigned char iconBuffer[] = {
            /*blue*/ 0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,
            /*red */ 255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255 };
        Image iconFromBuffer(&iconBuffer[0], iconBufferSize, Math::Size2I(4, 2), ImageChannels::RGBAlpha);

        _mainWindow.SetIcon(iconFromBuffer);
    }

    void MainFrameListener::SetCustomCursor()
    {
        const auto cursor = _windowBackend->AddCursor("test cursor", Utils::Concatenate(KMP_ICONS_FOLDER, "test_cursor.png"));

        if (cursor)
        {
            _mainWindow.SetCursor(*cursor);
        }
    }

    void MainFrameListener::SwitchFonts()
    {
        _switchFontRequested = true;
        _useDefaultFont = !_useDefaultFont;
    }

    void MainFrameListener::Render()
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

        RenderEventsWindow(applicationWindowFlags);
        RenderControlsWindow(applicationWindowFlags);
        RenderInfoWindow(applicationWindowFlags);
        RenderFrameListenersWindow(applicationWindowFlags);
        RenderActionEventsEmulatorWindow(applicationWindowFlags);

        ImGui::End(); //Id_Dockspace

        _imguiImpl->Render();
        ImGui::EndFrame();

        _sharedState.updateMask = 0;
        _sharedState.updateMaskString = "";
    }

    void MainFrameListener::RenderEventsWindow(int windowFlags)
    {
        ImGui::Begin(Id_EventsWindow, nullptr, windowFlags);
        {
            const auto disableGuard = ImGuiUtils::DisableGuard(true);
            ImGui::Checkbox("KeyPress", reinterpret_cast<bool*>(&_keyPressEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _keyPressEventInvokedCount);

            ImGui::Checkbox("KeyRelease", reinterpret_cast<bool*>(&_keyReleaseEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _keyReleaseEventInvokedCount);

            ImGui::Checkbox("KeyChar", reinterpret_cast<bool*>(&_keyCharEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _keyCharEventInvokedCount);

            ImGui::Separator();
            ImGui::Checkbox("MouseMove", reinterpret_cast<bool*>(&_mouseMoveEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _mouseMoveEventInvokedCount);

            ImGui::Checkbox("MouseScroll", reinterpret_cast<bool*>(&_mouseScrollEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _mouseScrollEventInvokedCount);

            ImGui::Checkbox("MousePress", reinterpret_cast<bool*>(&_mouseButtonPressEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _mouseButtonPressEventInvokedCount);

            ImGui::Checkbox("MouseRelease", reinterpret_cast<bool*>(&_mouseButtonReleaseEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _mouseButtonReleaseEventInvokedCount);

            ImGui::Separator();
            ImGui::Checkbox("WindowMove", reinterpret_cast<bool*>(&_windowMoveEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _windowMoveEventInvokedCount);

            ImGui::Checkbox("WindowResize", reinterpret_cast<bool*>(&_windowResizeEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _windowResizeEventInvokedCount);

            ImGui::Checkbox("WindowFocus", reinterpret_cast<bool*>(&_windowFocusEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _windowFocusEventInvokedCount);

            ImGui::Checkbox("WindowIconify", reinterpret_cast<bool*>(&_windowIconifyEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _windowIconifyEventInvokedCount);

            ImGui::Checkbox("FramebufferRefresh", reinterpret_cast<bool*>(&_windowFramebufferRefreshEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _windowFramebufferRefreshEventInvokedCount);

            ImGui::Checkbox("FramebufferResize", reinterpret_cast<bool*>(&_windowFramebufferResizeEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _windowFramebufferResizeEventInvokedCount);

            ImGui::Separator();
            ImGui::Checkbox("CustomEvent", reinterpret_cast<bool*>(&_customEventInvokedCount));
            ImGui::SameLine();
            ImGui::Text("%d", _customEventInvokedCount);
        }
        ImGui::End(); //Id_EventsWindow
    }

    void MainFrameListener::RenderControlsWindow(int windowFlags)
    {
        ImGui::Begin(Id_ControlsWindow, nullptr, windowFlags);
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
    }

    void MainFrameListener::RenderInfoWindow(int windowFlags)
    {
        ImGui::Begin(Id_InfoWindow, nullptr, windowFlags);
        {
            ImGuiStyle& style = ImGui::GetStyle();
            const auto oldTextColor = style.Colors[ImGuiCol_Text];
            style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            ImGui::Text("Press Ctrl+LMB/RMB to changed cursor mode");
            style.Colors[ImGuiCol_Text] = oldTextColor;


            const ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);


            const auto windowPos = _mainWindow.GetPosition();
            ImGui::Text("Window position: [%4d : %4d]", windowPos.x, windowPos.y);
            ImGui::SameLine(0.0f, 32.0f);

            const auto windowSize = _mainWindow.GetSize();
            ImGui::Text("Window size: [%4d x %4d]", windowSize.x, windowSize.y);


            const auto dpi = _mainWindow.GetDPI();
            ImGui::Text("DPI: %d", dpi);
            ImGui::SameLine(0.0f, 32.0f);

            const auto dpiScale = _mainWindow.GetDPIScale();
            ImGui::Text("DPI scale: %.2f", dpiScale);
            ImGui::SameLine(0.0f, 32.0f);

            const auto screenModeStr = Window::ScreenModeToString(_mainWindow.GetScreenMode());
            ImGui::Text("Screen mode: %s", screenModeStr.c_str());


            const auto cursorPosition = _mainWindow.GetCursorPosition();
            ImGui::Text("Mouse position: [%4d : %4d]", cursorPosition.x, cursorPosition.y);
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

            const auto keyMods = _inputManager->GetKeyModifiersMask();
            StringVector keyModsStringVector;
            if (keyMods & Input::Modifier::Alt)
            {
                keyModsStringVector.push_back(Input::GetKeyModifierName(Input::Modifier::Alt));
            }
            if (keyMods & Input::Modifier::Shift)
            {
                keyModsStringVector.push_back(Input::GetKeyModifierName(Input::Modifier::Shift));
            }
            if (keyMods & Input::Modifier::Ctrl)
            {
                keyModsStringVector.push_back(Input::GetKeyModifierName(Input::Modifier::Ctrl));
            }
            if (keyMods & Input::Modifier::Super)
            {
                keyModsStringVector.push_back(Input::GetKeyModifierName(Input::Modifier::Super));
            }
            String keyModsString = "";
            if (!keyModsStringVector.empty())
            {
                keyModsString = Utils::StringVectorToString(keyModsStringVector, '+', false);
            }
            ImGui::Text("Key mods: %s", keyModsString.c_str());

            StringVector mouseButtonsPressed;
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button0))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button0));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button1))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button1));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button2))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button2));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button3))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button3));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button4))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button4));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button5))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button5));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button6))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button6));
            }
            if (_inputManager->IsMouseButtonPressed(Input::Code::Mouse_Button7))
            {
                mouseButtonsPressed.push_back(Input::GetCodeName(Input::Code::Mouse_Button7));
            }
            String mouseButtonsString = "";
            if (!mouseButtonsPressed.empty())
            {
                mouseButtonsString = Utils::StringVectorToString(mouseButtonsPressed, '+', false);
            }
            ImGui::Text("Mouse buttons pressed: %s", mouseButtonsString.c_str());

            StringVector keysPressedStringVector;
            String keysPressedString = "";
            for (const auto& key : _keysPressed)
            {
                keysPressedStringVector.push_back(Input::GetCodeName(key));
            }
            if (!keysPressedStringVector.empty())
            {
                keysPressedString = Utils::StringVectorToString(keysPressedStringVector, ',', false);
            }
            ImGui::Text("Keys pressed: %s", keysPressedString.c_str());
        }
        ImGui::End(); //Id_InfoWindow
    }

    void MainFrameListener::RenderFrameListenersWindow(int windowFlags)
    {
        ImGui::Begin(Id_FrameListenersWindow, nullptr, windowFlags);
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
            ImGui::SameLine();
            {
                const auto disableGuard = ImGuiUtils::DisableGuard(true);
                ImGui::Checkbox("Delegate alive", &_sharedState.frame1DelegateAlive);
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
            if (_mouseButtonHandlersColoring)
            {
                ImGuiStyle& style = ImGui::GetStyle();
                const auto oldTextColor = style.Colors[ImGuiCol_Text];
                style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
                ImGui::Text("OnMouseClickEvent order: %s", _sharedState.eventProcessingString.c_str());
                style.Colors[ImGuiCol_Text] = oldTextColor;
            }
            else
            {
                ImGui::Text("OnMouseClickEvent order: %s", _sharedState.eventProcessingString.c_str());
            }

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
            

            ImGui::SeparatorText("Main frame listener functions");
            if (ImGui::Button("Activate mouse button click handler"))
            {
                SetMouseClickHandlerActive(true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Deactivate mouse button click handler"))
            {
                SetMouseClickHandlerActive(false);
            }
            ImGui::SameLine();
            if (ImGui::Button("Trigger custom event"))
            {
                Events::QueueEvent(CreateUPtr<Events::CustomEvent>());
            }
        }
        ImGui::End(); //Id_FrameListenersWindow
    }

    void MainFrameListener::RenderActionEventsEmulatorWindow(int windowFlags)
    {
        ImGui::Begin(Id_ActionEventsEmulatorWindow, nullptr, windowFlags);
        {
            if (ImGui::RadioButton("Arrows", _emulatorMoveWASD == 0))
            {
                _emulatorMoveWASD = 0;
                _inputManager->RemapInputToAction({Input::Code::Key_Up, Input::PressNoModsCondition}, "move_forward"_sid);
                _inputManager->RemapInputToAction({Input::Code::Key_Left, Input::PressNoModsCondition}, "move_left"_sid);
                _inputManager->RemapInputToAction({Input::Code::Key_Down, Input::PressNoModsCondition}, "move_backward"_sid);
                _inputManager->RemapInputToAction({Input::Code::Key_Right, Input::PressNoModsCondition}, "move_right"_sid);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("WASD", _emulatorMoveWASD == 1))
            {
                _emulatorMoveWASD = 1;
                _inputManager->RemapInputToAction({Input::Code::Key_W, Input::PressNoModsCondition}, "move_forward"_sid);
                _inputManager->RemapInputToAction({Input::Code::Key_A, Input::PressNoModsCondition}, "move_left"_sid);
                _inputManager->RemapInputToAction({Input::Code::Key_S, Input::PressNoModsCondition}, "move_backward"_sid);
                _inputManager->RemapInputToAction({Input::Code::Key_D, Input::PressNoModsCondition}, "move_right"_sid);
            }
            ImGui::SameLine();
            ImGui::Text("Player position: [%3d : %3d]", _emulatorPlayerPos.x / 5, _emulatorPlayerPos.y / 5);
            ImGui::SameLine();
            ImGui::Text("Crouching: %s", _emulatorPlayerCrouching ? "true" : "false");
            
            ImGui::Text("Mouse position (get): [%4d : %4d]", _emulatorMousePosGet.x, _emulatorMousePosGet.y);
            ImGui::SameLine();
            ImGui::Text("Mouse position (callback): [%4d : %4d]", _emulatorMousePosCb.x, _emulatorMousePosCb.y);

            ImGui::Text("Mouse move (get): [%3d : %3d]", _emulatorMouseMoveGet.x, _emulatorMouseMoveGet.y);
            ImGui::SameLine();
            ImGui::Text("Mouse move (callback): [%3d : %3d]", _emulatorMouseMoveCb.x, _emulatorMouseMoveCb.y);
        }
        ImGui::End(); //Id_ActionEventsEmulatorWindow
    }

    bool MainFrameListener::MousePositionIsNotZero() const
    {
        const auto cursorPosition = _mainWindow.GetCursorPosition();
        return cursorPosition.x != 0 || cursorPosition.y != 0;
    }

    bool MainFrameListener::DPIIsNotZero() const
    {
        return _mainWindow.GetDPI() > 0;
    }

    bool MainFrameListener::DPIScaleIsNotZero() const
    {
        return _mainWindow.GetDPIScale() > 0.0f;
    }

    bool MainFrameListener::DefaultSizeIsNotZero() const
    {
        const auto size = _mainWindow.GetSize();
        return size.x > 0 && size.y > 0;
    }

    bool MainFrameListener::DefaultWindowedSizeIsNotZero() const
    {
        const auto windowedSize = _mainWindow.GetWindowedSize();
        return windowedSize.x > 0 && windowedSize.y > 0;
    }

    void MainFrameListener::SetMouseClickHandlerActive(bool active)
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

    bool MainFrameListener::OnKeyPressEvent(Events::KeyPressEvent& event)
    {
        _keyPressEventInvokedCount++;
        _keysPressed.insert(event.GetKeyCode());
        return true;
    }

    bool MainFrameListener::OnKeyReleaseEvent(Events::KeyReleaseEvent& event)
    {
        _keyReleaseEventInvokedCount++;
        _keysPressed.erase(std::find(_keysPressed.begin(), _keysPressed.end(), event.GetKeyCode()));
        return true;
    }

    bool MainFrameListener::OnMouseButtonPressEvent(Events::MouseButtonPressEvent& evt)
    {
        _mouseButtonPressEventInvokedCount++;
        if (evt.GetMouseButton() == Input::Code::Mouse_ButtonLeft && evt.GetMods() & Input::Modifier::Ctrl)
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
        else if (evt.GetMouseButton() == Input::Code::Mouse_ButtonRight && evt.GetMods() & Input::Modifier::Ctrl)
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

        _sharedState.eventProcessingString = "M";
        _mouseButtonHandlersColoring = true;

        return false;
    }

    bool MainFrameListener::OnMouseButtonReleaseEvent(Events::MouseButtonReleaseEvent&)
    {
        _mouseButtonReleaseEventInvokedCount++;
        _mouseButtonHandlersColoring = false;
        return true;
    }

    bool MainFrameListener::OnWindowFramebufferRefreshEvent(Events::WindowFramebufferRefreshEvent&)
    {
        _windowFramebufferRefreshEventInvokedCount++;
        Render();
        return true;
    }

    bool MainFrameListener::OnWindowContentScaleEvent(Events::WindowContentScaleEvent& evt)
    {
        const auto scale = evt.GetScale();

        _imguiImpl.reset();
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend->GetType()), true, true, "imgui_test_app.ini"));

        const auto& defaultFontAsset = _assetsManager->GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), scale, 15);
        _imguiImpl->Stylize(scale);

        return true;
    }

    bool MainFrameListener::OnWindowCloseEvent(Events::WindowCloseEvent&)
    {
        _mainWindow.SetShouldClose(true);
        _windowCloseEventInvokedCount++;
        return true;
    }
}