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

    EditorUI::EditorUI(Window& mainWindow, float dpiScale, GraphicsBackend& graphicsBackend, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager)
        : _systemMetricsManager(systemMetricsManager)
        , _dpiScale(dpiScale)
        , _uiImpl(nullptr)
        , _compositor(CreateUPtr<EditorUICompositor>(mainWindow, _dpiScale, graphicsBackend, localizationManager, systemMetricsManager))
        , _metricsTimer(1000)
    {
        Initialize(mainWindow, graphicsBackend.GetType());
    }
    //--------------------------------------------------------------------------

    EditorUI::~EditorUI()
    {
        _uiImpl.reset();
        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorUI::Initialize(Window& mainWindow, GraphicsBackendType graphicsBackendType)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        _uiImpl.reset(EditorUIImpl::CreateImpl(mainWindow, graphicsBackendType));

        AddDefaultFont();
        AddIconsFont();
        Stylize();

        _metricsTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void EditorUI::AddDefaultFont() const
    {
        auto& io = ImGui::GetIO();
        const auto fontSize = 18 * _dpiScale;
        const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    //--------------------------------------------------------------------------

    void EditorUI::AddIconsFont() const
    {
        auto& io = ImGui::GetIO();
        const auto fontSize = 18 * _dpiScale;
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.GlyphMinAdvanceX = fontSize;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphOffset = ImVec2(0.0f, 0.0f);
        static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
        io.Fonts->AddFontFromFileTTF(Utils::Concatenate(KMP_FONTS_FOLDER, "forkawesome-webfont.ttf").c_str(), fontSize, &iconsConfig, iconsRanges);
    }
    //--------------------------------------------------------------------------

    void EditorUI::Stylize() const
    {
        auto& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.DisabledAlpha = 0.4f;
        style.ScaleAllSizes(_dpiScale);
    }
    //--------------------------------------------------------------------------

    void EditorUI::Finalize() const
    {
        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoopIteration()
    {
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
        _uiImpl->NewFrame();
        ImGui::NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUI::BeginApplicationArea() const
    {
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
        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowViewport(viewport->ID);

        {
            UiUtils::StyleVarGuard guard({
                {ImGuiStyleVar_WindowRounding, 0.0f},
                {ImGuiStyleVar_WindowBorderSize, 0.0f},
                {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
            });

            const static auto statusBarHeight = 28 * _dpiScale;
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
        _compositor->ComposeMainArea();
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndMainWorkingArea() const
    {
        ImGui::EndChild(); // IdApp_MainWorkingArea
    }
    //--------------------------------------------------------------------------

    void EditorUI::BeginStatusBarArea() const
    {
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
        _compositor->ComposeStatusBar(_metricsTimer);
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndStatusBarArea() const
    {
        ImGui::EndChild(); // IdApp_StatusBar
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndApplicationArea() const
    {
        ImGui::End(); // IdApp_ApplicationWindow
    }
    //--------------------------------------------------------------------------

    void EditorUI::Render()
    {
        ImGui::Render();
        _uiImpl->Render();
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndFrame() const
    {
        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        return _compositor->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        LoopIteration();
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnKeyPressEvent(KeyPressEvent& event)
    {
        return _compositor->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorUI::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(SettingsEntryName);
        settings.SaveUInt(MetricsTimeoutStr, _metricsTimer.GetTimeout());
        _compositor->SaveSettings(settings);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(SettingsEntryName);
        _metricsTimer.SetTimeout(settings.GetUInt(MetricsTimeoutStr, 1000));
        _compositor->LoadSettings(settings);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------
}