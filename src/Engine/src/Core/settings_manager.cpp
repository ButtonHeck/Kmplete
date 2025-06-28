#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Json/json_document.h"
#include "Kmplete/Filesystem/filesystem.h"

namespace Kmplete
{
    SettingsManager::SettingsManager(const Path& filename)
        : _filename(filename)
    {}
    //--------------------------------------------------------------------------

    Nullable<Settings*> SettingsManager::PutSettings(const String& name)
    {
        if (_namedSettings.contains(name))
        {
            _namedSettings.erase(name);
        }

        _namedSettings.insert({ name, CreateUPtr<Settings>(name) });
        return GetSettings(name);
    }
    //--------------------------------------------------------------------------

    Nullable<Settings*> SettingsManager::GetSettings(const String& name) const
    {
        if (_namedSettings.contains(name))
        {
            return _namedSettings.at(name).get();
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::LoadSettings()
    {
        JsonDocument document(_filename);
        if (document.HasError())
        {
            KMP_LOG_WARN("SettingsManager: failed to load settings from '{}'", _filename);
            return false;
        }

        const auto documentChildren = document.GetChildren();
        for (const auto& [name, childDocument] : documentChildren)
        {
            if (_namedSettings.contains(name))
            {
                _namedSettings.erase(name);
            }

            _namedSettings.insert({ name, CreateUPtr<Settings>(name, childDocument) });
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::SaveSettings() const
    {
        JsonDocument summaryDocument;

        for (const auto& [settingsEntryName, settingsEntry] : _namedSettings)
        {
            summaryDocument.AddChildDocument(settingsEntryName, settingsEntry->GetDocument());
        }

        return summaryDocument.Save(_filename);
    }
    //--------------------------------------------------------------------------

    void SettingsManager::SetFilename(const Path& filename) noexcept
    {
        _filename = filename;
    }
    //--------------------------------------------------------------------------

    const Path& SettingsManager::GetFilename() const noexcept
    {
        return _filename;
    }
    //--------------------------------------------------------------------------
}
