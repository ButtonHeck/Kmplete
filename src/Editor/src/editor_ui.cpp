#include "editor_ui.h"
#include "ui_utils.h"
#include "ui_identifiers.h"
#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Graphics/graphics_backend.h"

#include <imgui.h>
#include <forkawesome-webfont.h>

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "EditorUI";
    constexpr static auto MetricsTimeoutStr = "MetricsTimeout";

    EditorUI::EditorUI(Window& mainWindow, GraphicsBackend& graphicsBackend, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager)
        :
#if defined(KMP_PROFILE)
        _constructorProfilerTimer(CreateUPtr<ProfilerTimer>("EditorUI::EditorUI(Window&, GraphicsBackend&, LocalizationManager&, SystemMetricsManager&)")),
#endif
        _systemMetricsManager(systemMetricsManager)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _uiImpl(nullptr)
        , _compositor(CreateUPtr<EditorUICompositor>(_mainWindow, _graphicsBackend, localizationManager, systemMetricsManager))
        , _metricsTimer(1000)
    {
        Initialize();

#if defined(KMP_PROFILE)
        _constructorProfilerTimer.reset(nullptr);
#endif
    }
    //--------------------------------------------------------------------------

    EditorUI::~EditorUI()
    {
        KMP_PROFILE_FUNCTION();

        _uiImpl.reset();
        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorUI::Initialize()
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

    void EditorUI::AddDefaultFont(float dpiScale) const
    {
        KMP_PROFILE_FUNCTION();

        auto& io = ImGui::GetIO();
        const auto fontSize = 18 * dpiScale;
        const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    //--------------------------------------------------------------------------

    void EditorUI::AddIconsFont(float dpiScale) const
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

    void EditorUI::Stylize(float dpiScale) const
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

    void EditorUI::Finalize() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoopIteration()
    {
        KMP_PROFILE_FUNCTION();

        if (_metricsTimer.ReachedTimeout())
        {
            _metricsTimer.Mark();
            _systemMetricsManager.Update();
        }

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
        Render();
        EndFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUI::NewFrame()
    {
        KMP_PROFILE_FUNCTION();

        _uiImpl->NewFrame();
        ImGui::NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUI::BeginApplicationArea() const
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

    void EditorUI::BeginMainWorkingArea() const
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

    void EditorUI::ComposeMainArea()
    {
        KMP_PROFILE_FUNCTION();

        _compositor->ComposeMainArea();
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndMainWorkingArea() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::EndChild(); // IdApp_MainWorkingArea
    }
    //--------------------------------------------------------------------------

    void EditorUI::BeginStatusBarArea() const
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

    void EditorUI::ComposeStatusBar()
    {
        KMP_PROFILE_FUNCTION();

        _compositor->ComposeStatusBar(_metricsTimer);
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::EndChild(); // IdApp_StatusBar
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndApplicationArea() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::End(); // IdApp_ApplicationWindow
    }
    //--------------------------------------------------------------------------

    void EditorUI::Render()
    {
        KMP_PROFILE_FUNCTION();

        ImGui::Render();
        _uiImpl->Render();
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndFrame() const
    {
        KMP_PROFILE_FUNCTION();

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _compositor->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        KMP_PROFILE_FUNCTION();

        LoopIteration();
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowContentScaleEvent(WindowContentScaleEvent& event)
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

    bool EditorUI::OnKeyPressEvent(KeyPressEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _compositor->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorUI::SaveSettings(Settings& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveUInt(MetricsTimeoutStr, _metricsTimer.GetTimeout());
        _compositor->SaveSettings(settings);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoadSettings(Settings& settings)
    {
        KMP_PROFILE_FUNCTION();

        settings.StartLoadObject(SettingsEntryName);
        _metricsTimer.SetTimeout(settings.GetUInt(MetricsTimeoutStr, 1000));
        _compositor->LoadSettings(settings);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------
}