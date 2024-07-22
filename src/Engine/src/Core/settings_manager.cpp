#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/document.h>
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
    {}
    //--------------------------------------------------------------------------

    bool SettingsManager::Initialize()
    {
        Log::CoreInfo("SettingsManager: loading from '{}'", Filesystem::ToGenericU8String(_filename));

        if (!Filesystem::PathExists(_filename))
        {
            Log::CoreWarn("SettingsManager: insufficient path");
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
            Log::CoreError("SettingsManager: JSON parsing error '{}'", rapidjson::GetParseError_En(document.GetParseError()));
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

    void SettingsManager::Finalize()
    {
        rapidjson::Document summaryDocument;
        summaryDocument.SetObject();

        for (const auto& settingsEntry : _settings)
        {
            if (settingsEntry.second->ParseToDocument())
            {
                auto& settingsEntryDocument = settingsEntry.second->GetDocument();
                summaryDocument.AddMember(rapidjson::GenericStringRef(settingsEntry.first.c_str()), settingsEntryDocument.GetObject(), settingsEntryDocument.GetAllocator());
            }
        }

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

        if (summaryDocument.Accept(writer))
        {
            std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
            if (!outputStream.is_open() || !outputStream.good())
            {
                Log::CoreWarn("SettingsManager: failed to open file stream");
                return;
            }

            outputStream << buffer.GetString();
            outputStream.close();

            Log::CoreInfo("SettingsManager: settings written successfully");
        }
        else
        {
            Log::CoreWarn("SettingsManager: failed to write settings");
        }
    }
    //--------------------------------------------------------------------------

    void SettingsManager::PutSettings(const std::string& name, const Ptr<Settings>& settings)
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
}

#ifdef KMP_UNDEF_GetObject
#pragma pop_macro("GetObject")
#undef KMP_UNDEF_GetObject
#endif