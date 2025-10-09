#include "editor_frame_listener.h"
#include "ui_utils.h"
#include "ui_identifiers.h"

#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Utils/function_utils.h"

#include <imgui.h>
#include <forkawesome-webfont.h>


namespace Kmplete
{
    constexpr static auto SettingsEntryName = "EditorFrameListener";

    EditorFrameListener::EditorFrameListener(Window& mainWindow, GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager, Timer& metricsTimer)
        : FrameListener("EditorFrameListener")
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("EditorFrameListener::EditorFrameListener(Window&, GraphicsBackend&, Assets::AssetsManager&, LocalizationManager&, SystemMetricsManager&)")
        , _systemMetricsManager(systemMetricsManager)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _uiImpl(nullptr)
        , _uiCompositor(CreateUPtr<EditorUICompositor>(_mainWindow, _graphicsBackend, _assetsManager, localizationManager, systemMetricsManager))
        , _metricsTimer(metricsTimer)
    {
        Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    EditorFrameListener::~EditorFrameListener()
    {
        KMP_PROFILE_FUNCTION();

        _uiImpl.reset();
        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        _uiImpl.reset(EditorUIImpl::CreateImpl(_mainWindow, _graphicsBackend.GetType()));

        const auto dpiScale = _mainWindow.GetDPIScale();
        AddDefaultFont(dpiScale);
        AddIconsFont(dpiScale);
        Stylize(dpiScale);

        _metricsTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::AddDefaultFont(float dpiScale) const
    {
        KMP_PROFILE_FUNCTION();

        auto& io = ImGui::GetIO();
        const auto fontSize = 18 * dpiScale;
        const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::AddIconsFont(float dpiScale) const
    {
        KMP_PROFILE_FUNCTION();

        auto& io = ImGui::GetIO();
        const auto fontSize = 18 * dpiScale;
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.GlyphMinAdvanceX = fontSize;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphOffset = ImVec2(0.0f, 0.0f);
        static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
        io.Fonts->AddFontFromFileTTF(Utils::Concatenate(KMP_FONTS_FOLDER, "forkawesome-webfont.ttf").c_str(), fontSize, &iconsConfig, iconsRanges);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Stylize(float dpiScale) const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::GetStyle() = ImGuiStyle();

        auto& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.DisabledAlpha = 0.4f;

        style.ScaleAllSizes(dpiScale);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Finalize() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Render()
    {
        KMP_PROFILE_FUNCTION();

        NewFrame();
        {
            BeginApplicationArea();
            {
                BeginMainWorkingArea();
                {
                    ComposeMainArea();

                    UiUtils::StyleVarGuard styleVarGuard({
                        {ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f)}
                    });
                    EndMainWorkingArea();
                }

                BeginStatusBarArea();
                {
                    ComposeStatusBar();
                }
                EndStatusBarArea();
            }
            EndApplicationArea();
        }

        ImGui::Render();
        _uiImpl->Render();
        EndFrame();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::OnEvent(Event& event)
    {
        KMP_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>(KMP_BIND(EditorFrameListener::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(KMP_BIND(EditorFrameListener::OnWindowFramebufferRefreshEvent));
        dispatcher.Dispatch<WindowContentScaleEvent>(KMP_BIND(EditorFrameListener::OnWindowContentScaleEvent));

        dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(EditorFrameListener::OnKeyPressEvent));
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _uiCompositor->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        KMP_PROFILE_FUNCTION();

        Render();
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnWindowContentScaleEvent(WindowContentScaleEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        _uiImpl.reset();
        _uiImpl.reset(EditorUIImpl::CreateImpl(_mainWindow, _graphicsBackend.GetType()));

        auto& io = ImGui::GetIO();
        io.Fonts->Clear();
        const auto scale = event.GetScale();
        AddDefaultFont(scale);
        AddIconsFont(scale);
        io.Fonts->Build();
        Stylize(scale);

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnKeyPressEvent(KeyPressEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _uiCompositor->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveObject(SettingsEntryName);
        _uiCompositor->SaveSettings(settings);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION();

        settings.StartLoadObject(SettingsEntryName);
        _uiCompositor->LoadSettings(settings);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::NewFrame()
    {
        KMP_PROFILE_FUNCTION();

        _uiImpl->NewFrame();
        ImGui::NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::BeginApplicationArea() const
    {
        KMP_PROFILE_FUNCTION();

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        UiUtils::StyleVarGuard guard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        constexpr static auto applicationWindowFlags =
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin(IdApp_ApplicationWindow, nullptr, applicationWindowFlags);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::BeginMainWorkingArea() const
    {
        KMP_PROFILE_FUNCTION();

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowViewport(viewport->ID);

        {
            UiUtils::StyleVarGuard guard({
                {ImGuiStyleVar_WindowRounding, 0.0f},
                {ImGuiStyleVar_WindowBorderSize, 0.0f},
                {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
            });

            const auto statusBarHeight = 28 * _mainWindow.GetDPIScale();
            constexpr static auto workingAreaFlags =
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::BeginChild(IdApp_MainWorkingArea, ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - statusBarHeight), false, workingAreaFlags);
        }

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto dockspaceId = ImGui::GetID(IdApp_EditorDockspace);
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::ComposeMainArea()
    {
        KMP_PROFILE_FUNCTION();

        _uiCompositor->ComposeMainArea();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndMainWorkingArea() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::EndChild(); // IdApp_MainWorkingArea
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::BeginStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION();

        UiUtils::StyleVarGuard styleVarGuard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        UiUtils::StyleColorGuard styleColorGuard({ { ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)} });

        constexpr static auto statusBarFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking;
        ImGui::BeginChild(IdApp_StatusBar, ImGui::GetContentRegionAvail(), false, statusBarFlags);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::ComposeStatusBar()
    {
        KMP_PROFILE_FUNCTION();

        _uiCompositor->ComposeStatusBar(_metricsTimer);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::EndChild(); // IdApp_StatusBar
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndApplicationArea() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::End(); // IdApp_ApplicationWindow
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndFrame() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------
}