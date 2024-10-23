#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Json/json_document.h"

namespace Kmplete
{
    SettingsManager::SettingsManager(const std::filesystem::path& filename)
        : _filename(filename)
    {}
    //--------------------------------------------------------------------------

    Settings* const SettingsManager::PutSettings(const std::string& name)
    {
        if (_settings.contains(name))
        {
            _settings.erase(name);
        }

        _settings.insert({ name, CreateUPtr<Settings>(name) });
        return GetSettings(name);
    }
    //--------------------------------------------------------------------------

    Settings* const SettingsManager::GetSettings(const std::string& name) const
    {
        if (_settings.contains(name))
        {
            return _settings.at(name).get();
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::LoadSettings()
    {
        JsonDocument document(_filename);
        if (document.HasError())
        {
            Log::CoreWarn("SettingsManager: failed to load settings from '{}'", Filesystem::ToGenericU8String(_filename));
            return false;
        }

        const auto documentChildren = document.GetChildren();
        for (const auto& [name, childDocument] : documentChildren)
        {
            if (_settings.contains(name))
            {
                _settings.erase(name);
            }

            _settings.insert({ name, CreateUPtr<Settings>(name, childDocument) });
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::SaveSettings() const
    {
        JsonDocument summaryDocument;

        for (const auto& [settingsEntryName, settingsEntry] : _settings)
        {
            summaryDocument.AddChildDocument(settingsEntryName, settingsEntry->GetDocument());
        }

        return summaryDocument.Save(_filename);
    }
    //--------------------------------------------------------------------------

    void SettingsManager::SetFilename(const std::filesystem::path& filename) KMP_NOEXCEPT
    {
        _filename = filename;
    }
    //--------------------------------------------------------------------------

    const std::filesystem::path& SettingsManager::GetFilename() const KMP_NOEXCEPT
    {
        return _filename;
    }
    //--------------------------------------------------------------------------
}
