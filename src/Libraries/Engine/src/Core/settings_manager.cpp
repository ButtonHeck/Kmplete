#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Json/json_document.h"
#include "Kmplete/Filesystem/filesystem.h"


namespace Kmplete
{
    SettingsManager::SettingsManager(const Filepath& filepath)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _filepath(filepath)
    {
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    OptionalRef<SettingsDocument> SettingsManager::PutSettingsDocument(const String& name) KMP_PROFILING(ProfileLevelImportantVerbose)
    {
        if (_namedSettingsDocuments.contains(name))
        {
            _namedSettingsDocuments.erase(name);
        }

        _namedSettingsDocuments.emplace(name, CreateUPtr<SettingsDocument>(name));
        return GetSettingsDocument(name);
    }}
    //--------------------------------------------------------------------------

    OptionalRef<SettingsDocument> SettingsManager::GetSettingsDocument(const String& name) const KMP_PROFILING(ProfileLevelImportantVerbose)
    {
        if (_namedSettingsDocuments.contains(name))
        {
            return std::ref(*_namedSettingsDocuments.at(name).get());
        }

        return std::nullopt;
    }}
    //--------------------------------------------------------------------------

    bool SettingsManager::LoadSettings() KMP_PROFILING(ProfileLevelImportant)
    {
        JsonDocument document(_filepath);
        if (document.HasError())
        {
            KMP_LOG_WARN("failed to load settings from '{}' - {}", _filepath, document.ErrorDescription());
            return false;
        }

        const auto documentChildren = document.GetChildren();
        for (const auto& [name, childDocument] : documentChildren)
        {
            if (_namedSettingsDocuments.contains(name))
            {
                _namedSettingsDocuments.erase(name);
            }

            _namedSettingsDocuments.emplace(name, CreateUPtr<SettingsDocument>(name, childDocument));
        }

        KMP_LOG_INFO("settings successfully loaded from '{}'", _filepath);

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SettingsManager::SaveSettings() const KMP_PROFILING(ProfileLevelImportant)
    {
        JsonDocument summaryDocument;

        for (const auto& [settingsEntryName, settingsEntry] : _namedSettingsDocuments)
        {
            summaryDocument.AddChildDocument(settingsEntryName, settingsEntry->GetDocument());
        }

        const auto ok = summaryDocument.Save(_filepath);
        if (ok)
        {
            KMP_LOG_INFO("settings successfully saved to '{}'", _filepath);
        }
        else
        {
            KMP_LOG_ERROR("failed to save settings to '{}'", _filepath);
        }

        return ok;
    }}
    //--------------------------------------------------------------------------

    void SettingsManager::SetFilepath(const Filepath& filepath) noexcept
    {
        _filepath = filepath;
    }
    //--------------------------------------------------------------------------

    const Filepath& SettingsManager::GetFilepath() const noexcept
    {
        return _filepath;
    }
    //--------------------------------------------------------------------------
}
