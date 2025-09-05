#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Json/json_document.h"
#include "Kmplete/Filesystem/filesystem.h"

namespace Kmplete
{
    SettingsManager::SettingsManager(const Filepath& filepath)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("SettingsManager::SettingsManager(const Filepath&)")
          _filepath(filepath)
    {
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    OptionalRef<Settings> SettingsManager::PutSettings(const String& name)
    {
        KMP_PROFILE_FUNCTION();

        if (_namedSettings.contains(name))
        {
            _namedSettings.erase(name);
        }

        _namedSettings.emplace(name, CreateUPtr<Settings>(name));
        return GetSettings(name);
    }
    //--------------------------------------------------------------------------

    OptionalRef<Settings> SettingsManager::GetSettings(const String& name) const
    {
        KMP_PROFILE_FUNCTION();

        if (_namedSettings.contains(name))
        {
            return std::ref(*_namedSettings.at(name).get());
        }

        return std::nullopt;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::LoadSettings()
    {
        KMP_PROFILE_FUNCTION();

        JsonDocument document(_filepath);
        if (document.HasError())
        {
            KMP_LOG_WARN("failed to load settings from '{}'", _filepath);
            return false;
        }

        const auto documentChildren = document.GetChildren();
        for (const auto& [name, childDocument] : documentChildren)
        {
            if (_namedSettings.contains(name))
            {
                _namedSettings.erase(name);
            }

            _namedSettings.emplace(name, CreateUPtr<Settings>(name, childDocument));
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::SaveSettings() const
    {
        KMP_PROFILE_FUNCTION();

        JsonDocument summaryDocument;

        for (const auto& [settingsEntryName, settingsEntry] : _namedSettings)
        {
            summaryDocument.AddChildDocument(settingsEntryName, settingsEntry->GetDocument());
        }

        return summaryDocument.Save(_filepath);
    }
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
