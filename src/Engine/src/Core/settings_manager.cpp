#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>

#include <fstream>

#ifdef GetObject
#pragma push_macro("GetObject")
#undef GetObject
#define KMP_UNDEF_GetObject
#endif

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
        Log::CoreInfo("SettingsManager: loading from '{}'", Filesystem::ToGenericU8String(_filename));

        if (!Filesystem::PathExists(_filename))
        {
            Log::CoreWarn("SettingsManager: failed to load settings due to insufficient path");
            return false;
        }

        std::ifstream inputStream(_filename);
        if (!inputStream.is_open() || !inputStream.good())
        {
            return false;
        }

        rapidjson::Document document;
        rapidjson::IStreamWrapper jsonStream(inputStream);
        document.ParseStream(jsonStream);
        inputStream.close();

        if (document.HasParseError())
        {
            Log::CoreError("SettingsManager: failed to load settings due to JSON parsing error '{}'", rapidjson::GetParseError_En(document.GetParseError()));
            return false;
        }

        for (auto settingsEntry = document.MemberBegin(); settingsEntry != document.MemberEnd(); settingsEntry++)
        {
            const auto settingsName = settingsEntry->name.GetString();
            rapidjson::Document settingsEntryDocument;
            settingsEntryDocument.CopyFrom(settingsEntry->value, settingsEntryDocument.GetAllocator());

            PutSettings(settingsName, CreatePtr<Settings>(settingsName, std::move(settingsEntryDocument)));
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::SaveSettings() const
    {
        auto summaryDocument = AssembleDocument();
        return WriteDocument(summaryDocument);
    }
    //--------------------------------------------------------------------------

    Ptr<rapidjson::Document> SettingsManager::AssembleDocument() const
    {
        auto summaryDocument = CreatePtr<rapidjson::Document>();
        summaryDocument->SetObject();

        for (const auto& settingsEntry : _settings)
        {
            auto& settingsEntryDocument = settingsEntry.second->GetDocument();
            summaryDocument->AddMember(rapidjson::GenericStringRef(settingsEntry.first.c_str()), settingsEntryDocument.GetObject(), settingsEntryDocument.GetAllocator());
        }

        return summaryDocument;
    }
    //--------------------------------------------------------------------------

    bool SettingsManager::WriteDocument(const Ptr<rapidjson::Document> document) const
    {
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

        if (document->Accept(writer))
        {
            std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
            if (!outputStream.is_open() || !outputStream.good())
            {
                Log::CoreWarn("SettingsManager: failed to open file stream");
                return false;
            }

            outputStream << buffer.GetString();
            outputStream.close();

            Log::CoreInfo("SettingsManager: settings written successfully");
            return true;
        }

        Log::CoreWarn("SettingsManager: failed to write settings");
        return false;
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

#ifdef KMP_UNDEF_GetObject
#pragma pop_macro("GetObject")
#undef KMP_UNDEF_GetObject
#endif