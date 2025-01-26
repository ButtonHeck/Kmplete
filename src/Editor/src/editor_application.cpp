#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Utils/function_utils.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication(const Kmplete::ProgramOptions& programOptions)
    {
        try
        {
            ApplicationParameters applicationParameters{
                .settingsPath = programOptions.GetSettingsFilePath()
            };

            return CreateUPtr<EditorApplication>(applicationParameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CLIENT_CRITICAL("EditorApplication: failed to create Editor instance");
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    EditorApplication::EditorApplication(const ApplicationParameters& applicationParameters)
        : WindowApplication(applicationParameters)
        , _ui(new EditorUI(_mainWindow))
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

        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Finalize()
    {
        SaveSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
        KMP_LOG_CLIENT_DEBUG("EditorApplication: main loop started...");
        while (!_mainWindow->ShouldClose())
        {
            _mainWindow->ProcessEvents();
            _mainWindow->SwapBuffers();
        }

        KMP_LOG_CLIENT_DEBUG("EditorApplication: main loop finished");
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings(const Path& path) const
    {
        SaveSettingsInternal();
        WindowApplication::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings(const Path& path)
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