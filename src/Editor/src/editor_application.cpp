#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Utils/function_utils.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication(const ProgramOptions& programOptions)
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            ApplicationParameters applicationParameters{
                .applicationName = "Kmplete Editor",
                .settingsPath = programOptions.GetSettingsFilePath()
            };

            return CreateUPtr<EditorApplication>(applicationParameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CRITICAL("EditorApplication: failed to create Editor instance");
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    constexpr static auto SettingsEntryName = "EditorApplication";

    EditorApplication::EditorApplication(const ApplicationParameters& applicationParameters)
        : WindowApplication(applicationParameters)
#if defined(KMP_PROFILE)
        , _constructorProfilerTimer(CreateUPtr<ProfilerTimer>("EditorApplication::EditorApplication(const ApplicationParameters&)"))
#endif
        , _ui(nullptr)
    {
        Initialize();

#if defined(KMP_PROFILE)
        _constructorProfilerTimer.reset(nullptr);
#endif
    }
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication()
    {
        KMP_PROFILE_FUNCTION();

        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        auto& mainWindow = _windowBackend->GetMainWindow();
        mainWindow.SetTitle(_applicationName.c_str());
        mainWindow.SetEventCallback(KMP_BIND(EditorApplication::OnEvent));

        _graphicsBackend->GetTextureManager().CreateTexture("_flag_russian"_sid, Utils::Concatenate(KMP_ICONS_FOLDER, "flag_russia_128.png"));
        _graphicsBackend->GetTextureManager().CreateTexture("_flag_usa"_sid, Utils::Concatenate(KMP_ICONS_FOLDER, "flag_usa_128.png"));

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_EDITOR);
        _ui.reset(new EditorUI(mainWindow, *_graphicsBackend, *_localizationManager, *_systemMetricsManager));

        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
        auto& mainWindow = _windowBackend->GetMainWindow();

        KMP_LOG_DEBUG("EditorApplication: main loop started...");
        while (!mainWindow.ShouldClose())
        {
            mainWindow.ProcessEvents();
            _ui->LoopIteration();
            mainWindow.SwapBuffers();
        }

        KMP_LOG_DEBUG("EditorApplication: main loop finished");
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings(const Path& path /*= Path()*/) const
    {
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();
        WindowApplication::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings(const Path& path /*= Path()*/)
    {
        KMP_PROFILE_FUNCTION();

        WindowApplication::LoadSettings(path);
        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::OnEvent(Event& event)
    {
        KMP_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>(KMP_BIND(EditorApplication::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(KMP_BIND(EditorApplication::OnWindowFramebufferRefreshEvent));
        dispatcher.Dispatch<WindowContentScaleEvent>(KMP_BIND(EditorApplication::OnWindowContentScaleEvent));

        dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(EditorApplication::OnKeyPressEvent));
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _ui->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _ui->OnWindowFramebufferRefreshEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowContentScaleEvent(WindowContentScaleEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _ui->OnWindowContentScaleEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyPressEvent(KeyPressEvent& event)
    {
        KMP_PROFILE_FUNCTION();

        return _ui->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettingsInternal() const
    {
        KMP_PROFILE_FUNCTION();

        auto settings = _settingsManager->PutSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("EditorApplication: failed to create settings entry for saving");
            return;
        }

        _ui->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettingsInternal()
    {
        KMP_PROFILE_FUNCTION();

        const auto settings = _settingsManager->GetSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("EditorApplication: failed to get setting entry for loading");
            return;
        }

        _ui->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}