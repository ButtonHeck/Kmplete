#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Log/log.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication(const ProgramOptions& programOptions)
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            ApplicationParameters applicationParameters{
                .applicationName = "Kmplete Editor",
                .settingsFilepath = programOptions.GetSettingsFilepath()
            };

            return CreateUPtr<EditorApplication>(applicationParameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create Editor instance");
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    constexpr static auto SettingsEntryName = "EditorApplication";

    EditorApplication::EditorApplication(const ApplicationParameters& applicationParameters)
        : WindowApplication(applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("EditorApplication::EditorApplication(const ApplicationParameters&)")
        , _mainWindow(_windowBackend->GetMainWindow())
        , _ui(nullptr)
    {
        Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
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

        _mainWindow.SetTitle(_applicationName.c_str());

        _graphicsBackend->GetTextureManager().CreateTexture("_flag_russian"_sid, Utils::Concatenate(KMP_ICONS_FOLDER, "flag_russia_128.png"));
        _graphicsBackend->GetTextureManager().CreateTexture("_flag_usa"_sid, Utils::Concatenate(KMP_ICONS_FOLDER, "flag_usa_128.png"));

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_EDITOR);
        _ui.reset(new EditorUI(_mainWindow, *_graphicsBackend, *_localizationManager, *_systemMetricsManager));

        LoadSettingsInternal();

        AddFrameListener(_ui.get());
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings(const Filepath& filepath /*= Filepath()*/) const
    {
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();
        WindowApplication::SaveSettings(filepath);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings(const Filepath& filepath /*= Filepath()*/)
    {
        KMP_PROFILE_FUNCTION();

        WindowApplication::LoadSettings(filepath);
        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettingsInternal() const
    {
        KMP_PROFILE_FUNCTION();

        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

        _ui->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettingsInternal()
    {
        KMP_PROFILE_FUNCTION();

        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get setting entry for loading");
            return;
        }

        _ui->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}