#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/function_utils.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication(const std::filesystem::path& settingsFilePath)
    {
        try
        {
            return CreateUPtr<EditorApplication>(settingsFilePath);
        }
        catch (const std::exception&)
        {
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    EditorApplication::EditorApplication(const std::filesystem::path& settingsFilePath, const std::string& defaultSettingsName)
        : WindowApplication(settingsFilePath, defaultSettingsName)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    std::string EditorApplication::GetApplicationName() const KMP_NOEXCEPT
    {
        return std::string("Kmplete Editor");
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Initialize()
    {
        _mainWindow->SetTitle(GetApplicationName());
        _mainWindow->SetEventCallback(KMP_BIND(EditorApplication::OnEvent));

        LoadSettings();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Finalize()
    {
        SaveSettings();
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

    void EditorApplication::SaveSettings(const std::filesystem::path& path) const
    {
        SaveSettingsInternal();
        WindowApplication::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings(const std::filesystem::path& path)
    {
        WindowApplication::LoadSettings(path);
        LoadSettingsInternal();
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

    void EditorApplication::SaveSettingsInternal() const
    {
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettingsInternal()
    {
    }
    //--------------------------------------------------------------------------
}