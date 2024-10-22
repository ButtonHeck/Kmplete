#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Json/json_document.h"

namespace Kmplete
{
    SettingsManager::SettingsManager(const std::filesystem::path& filename)
        : _filename(filename)
    {
        Log::CoreTrace("SettingsManager: created '{}'", Filesystem::ToGenericU8String(_filename));
    }
    //--------------------------------------------------------------------------

    SettingsManager::~SettingsManager()
    {
        Log::CoreTrace("SettingsManager: destroyed '{}'", Filesystem::ToGenericU8String(_filename));
    }
    //--------------------------------------------------------------------------

    void SettingsManager::PutSettings(const std::string& name, const Ptr<Settings> settings)
    {
        if (_settings.contains(name))
        {
            _settings.erase(name);
        }

        _settings.insert({ name, settings });
    }
    //--------------------------------------------------------------------------

    Ptr<Settings> SettingsManager::PutSettings(const std::string& name)
    {
        auto settings = CreatePtr<Settings>(name);
        PutSettings(name, settings);
        return settings;
    }
    //--------------------------------------------------------------------------

    Ptr<Settings> SettingsManager::GetSettings(const std::string& name) const
    {
        if (_settings.contains(name))
        {
            return _settings.at(name);
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
            PutSettings(name, CreatePtr<Settings>(name, childDocument));
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::SaveSettings() const
    {
        JsonDocument summaryDocument;

        for (const auto& [settingsEntryName, settingsEntryDocument] : _settings)
        {
            const auto& settingEntryDocument = settingsEntryDocument->GetDocument();
            summaryDocument.AddChildDocument(settingsEntryName, *settingEntryDocument);
        }

        return summaryDocument.Save(_filename);
    }
    //--------------------------------------------------------------------------

    void SettingsManager::SetFilename(const std::filesystem::path& filename) KMP_NOEXCEPT
    {
        _filename = filename;
    }
    //--------------------------------------------------------------------------

    std::filesystem::path SettingsManager::GetFilename() const KMP_NOEXCEPT
    {
        return _filename;
    }
    //--------------------------------------------------------------------------
}
