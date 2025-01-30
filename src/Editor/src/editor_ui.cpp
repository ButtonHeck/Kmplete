#include "editor_ui.h"
#include "ui_utils.h"
#include "ui_identifiers.h"

#include <imgui.h>

namespace Kmplete
{
    constexpr static auto EditorUISettingsEntryName = "EditorUI";

    EditorUI::EditorUI(const Ptr<Window> window)
        : _uiImpl(nullptr)
        , _compositor(CreateUPtr<EditorUICompositor>(window))
    {
        Initialize(window);
    }
    //--------------------------------------------------------------------------

    EditorUI::~EditorUI()
    {
        _uiImpl.reset();
        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorUI::Initialize(const Ptr<Window> window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        _uiImpl.reset(EditorUIImpl::CreateImpl(window));

        AddDefaultFont();
        Stylize();
    }
    //--------------------------------------------------------------------------

    void EditorUI::AddDefaultFont() const
    {
        auto& io = ImGui::GetIO();
        const auto fontSize = 18;
        const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "/OpenSans-Regular.ttf");
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    //--------------------------------------------------------------------------

    void EditorUI::Stylize() const
    {
        auto& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.DisabledAlpha = 0.4f;
    }
    //--------------------------------------------------------------------------

    void EditorUI::Finalize() const
    {
        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoopIteration()
    {
        NewFrame();
        {
            BeginApplicationArea();
            {
                BeginMainWorkingArea();
                {
                    ComposeMainArea();
                }
                EndMainWorkingArea();

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

            constexpr static auto statusBarHeight = 40;
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
        _compositor->ComposeStatusBar();
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
        settings.StartSaveObject(EditorUISettingsEntryName);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(EditorUISettingsEntryName);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------
}