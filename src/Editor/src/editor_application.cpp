#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/function_utils.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication()
    {
        return CreateUPtr<EditorApplication>();
    }
    //--------------------------------------------------------------------------

    EditorApplication::EditorApplication()
        : WindowApplication()
    {}
    //--------------------------------------------------------------------------

    std::string EditorApplication::GetApplicationName() const KMP_NOEXCEPT
    {
        return std::string("Kmplete Editor");
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::Initialize(const std::string& settingsFilePath, const std::string& defaultSettingsName)
    {
        if (!WindowApplication::Initialize(settingsFilePath, defaultSettingsName))
        {
            return false;
        }

        _mainWindow->SetTitle(GetApplicationName());
        _mainWindow->SetEventCallback(KMP_BIND(EditorApplication::OnEvent));

        LoadSettings();

        return true;
    }

    void EditorApplication::Finalize()
    {
        SaveSettings();

        WindowApplication::Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
        while (!_mainWindow->ShouldClose())
        {
            _mainWindow->ProcessEvents();
            _mainWindow->SwapBuffers();
        }
    }
    //--------------------------------------------------------------------------

    void EditorApplication::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>(KMP_BIND(EditorApplication::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(KMP_BIND(EditorApplication::OnWindowFramebufferRefreshEvent));

        dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(EditorApplication::OnKeyPressEvent));
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowCloseEvent(WindowCloseEvent&)
    {
        _mainWindow->SetShouldClose(true);
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyPressEvent(KeyPressEvent&)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings() const
    {
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings()
    {
    }
    //--------------------------------------------------------------------------
}