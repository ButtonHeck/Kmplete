#include "editor_ui.h"
#include "ui_utils.h"

#include <imgui.h>

namespace Kmplete
{
    constexpr static auto EditorUISettingsEntryName = "EditorUI";

    EditorUI::EditorUI(const Ptr<Window> window)
        : _uiImpl(nullptr)
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
    }
    //--------------------------------------------------------------------------

    void EditorUI::Finalize()
    {
        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorUI::LoopIteration()
    {
        NewFrame();
        Stylize();
        BeginApplicationArea();
        BeginMainWorkingArea();
        Compose();
        EndMainWorkingArea();
        ComposeStatusBar();
        EndApplicationArea();
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

    void EditorUI::Stylize()
    {
        auto& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.DisabledAlpha = 0.4f;
    }
    //--------------------------------------------------------------------------

    void EditorUI::BeginApplicationArea()
    {
        const auto applicationWindowFlags =
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        UiUtils::StyleVarGuard guard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });
        ImGui::Begin("ApplicationWindow", nullptr, applicationWindowFlags);
    }
    //--------------------------------------------------------------------------

    void EditorUI::BeginMainWorkingArea()
    {
        constexpr static auto statusBarHeight = 40;
        const auto workingAreaFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowViewport(viewport->ID);

        {
            UiUtils::StyleVarGuard guard({
                {ImGuiStyleVar_WindowRounding, 0.0f},
                {ImGuiStyleVar_WindowBorderSize, 0.0f},
                {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
            });
            ImGui::BeginChild("MainWorkingArea", ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - statusBarHeight), false, workingAreaFlags);
        }

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto dockspaceId = ImGui::GetID("EditorDockspace");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUI::Compose()
    {
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndMainWorkingArea()
    {
        ImGui::EndChild(); // "MainWorkingArea"
    }
    //--------------------------------------------------------------------------

    void EditorUI::ComposeStatusBar()
    {
        UiUtils::StyleVarGuard styleVarGuard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        UiUtils::StyleColorGuard styleColorGuard({ { ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)} });

        if (ImGui::BeginChild("StatusBar", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
        {
            ImGui::EndChild();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndApplicationArea()
    {
        ImGui::End(); // "ApplicationWindow"
    }
    //--------------------------------------------------------------------------

    void EditorUI::Render()
    {
        ImGui::Render();
        _uiImpl->Render();
    }
    //--------------------------------------------------------------------------

    void EditorUI::EndFrame()
    {
        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowCloseEvent(WindowCloseEvent&)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        LoopIteration();
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUI::OnKeyPressEvent(KeyPressEvent&)
    {
        return true;
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