#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "shortcuts.h"
#include "Kmplete/Core/filesystem.h"

#include <imgui.h>
#include <imgui_internal.h> // for ImGui::DockBuilder api
#include <misc/cpp/imgui_stdlib.h> // for ImGui::InputText wrappers for std::string

namespace Kmplete
{
    EditorUICompositor::EditorUICompositor(const Ptr<Window> window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMainArea()
    {
        if (!Filesystem::PathExists(Filesystem::GetCurrentPath().append(ImGui::GetIO().IniFilename)))
        {
            ComposeDefaultLayout();
        }

        ComposeMenu();

        ComposePopups();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeDefaultLayout()
    {
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            ComposeMenuFile();
            ComposeMenuView();

            ImGui::EndMenuBar();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFile()
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Separator();

            ComposeMenuFileQuit();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuView()
    {
        if (ImGui::BeginMenu("View"))
        {
            ComposeMenuViewFullscreen();
            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFileQuit()
    {
        if (ImGui::MenuItem("Quit", Shortcuts::Quit.text))
        {
            _popups.quit = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuViewFullscreen()
    {
        const auto screenMode = _window->GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;
        if (ImGui::MenuItem("Fullscreen", Shortcuts::Fullscreen.text, &isFullscreen))
        {
            _window->SetScreenMode(isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposePopups()
    {
        if (_popups.quit)
        {
            PopupQuit();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::PopupQuit()
    {
        if (false /*future logic*/)
        {
        }
        else
        {
            _window->SetShouldClose(true);
            _popups.quit = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::SwitchFullscreen()
    {
        const auto isFullscreen = _window->GetScreenMode() == Window::WindowedFullscreenMode;
        _window->SetScreenMode(!isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeStatusBar()
    {
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnWindowCloseEvent(WindowCloseEvent&)
    {
        _popups.quit = true;
        _window->SetShouldClose(false);
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnKeyPressEvent(KeyPressEvent& event)
    {
        const auto keyCode = event.GetKeyCode();
        const auto mods = event.GetMods();

        if (event.IsRepeat())
        {
            return true;
        }

        if (Shortcuts::Quit.Accept(mods, keyCode))
        {
            _popups.quit = true;
        }
        else if (Shortcuts::Fullscreen.Accept(mods, keyCode))
        {
            SwitchFullscreen();
        }

        return true;
    }
    //--------------------------------------------------------------------------
}