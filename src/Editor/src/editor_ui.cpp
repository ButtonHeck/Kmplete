#include "editor_ui.h"

#include <imgui.h>

namespace Kmplete
{
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
}