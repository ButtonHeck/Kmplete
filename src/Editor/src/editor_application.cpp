#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Utils/function_utils.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication(const ProgramOptions& programOptions)
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

    constexpr static auto SettingsEntryName = "EditorApplication";

    EditorApplication::EditorApplication(const ApplicationParameters& applicationParameters)
        : WindowApplication(applicationParameters)
        , _ui(nullptr)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    std::string EditorApplication::GetApplicationName() const noexcept
    {
        return std::string("Kmplete Editor");
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Initialize()
    {
        auto& mainWindow = _windowBackend->GetMainWindow();
        mainWindow.SetTitle(GetApplicationName());
        mainWindow.SetEventCallback(KMP_BIND(EditorApplication::OnEvent));

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_EDITOR);
        _ui.reset(new EditorUI(mainWindow, _windowBackend->GetDPIScale(), *_localizationManager, *_systemMetricsManager, _graphicsBackend->GetType()));

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
        auto& mainWindow = _windowBackend->GetMainWindow();

        KMP_LOG_CLIENT_DEBUG("EditorApplication: main loop started...");
        while (!mainWindow.ShouldClose())
        {
            mainWindow.ProcessEvents();
            _ui->LoopIteration();
            mainWindow.SwapBuffers();
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

    bool EditorApplication::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        return _ui->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event)
    {
        return _ui->OnWindowFramebufferRefreshEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyPressEvent(KeyPressEvent& event)
    {
        return _ui->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettingsInternal() const
    {
        auto settings = _settingsManager->PutSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_CLIENT_WARN("EditorApplication: failed to create settings entry for saving");
            return;
        }

        _ui->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettingsInternal()
    {
        const auto settings = _settingsManager->GetSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_CLIENT_WARN("EditorApplication: failed to get setting entry for loading");
            return;
        }

        _ui->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}