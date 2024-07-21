#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <fstream>

#ifdef GetObject
#undef GetObject
#endif

namespace Kmplete
{
    SettingsManager::SettingsManager(const std::filesystem::path& filename)
        : _filename(filename)
    {}
    //--------------------------------------------------------------------------

    SettingsManager::~SettingsManager()
    {}
    //--------------------------------------------------------------------------

    bool SettingsManager::Initialize()
    {
        Log::CoreInfo("JsonReader: loading from '{}'", Filesystem::ToGenericU8String(_filename));

        if (!Filesystem::PathExists(_filename))
        {
            Log::CoreWarn("JsonReader: insufficient path");
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
            Log::CoreError("JsonReader: JSON parsing error '{}'", rapidjson::GetParseError_En(document.GetParseError()));
            return false;
        }

        for (auto entry = document.MemberBegin(); entry != document.MemberEnd(); entry++)
        {
            const auto entryName = entry->name.GetString();
            rapidjson::Document subDocument;
            subDocument.CopyFrom(entry->value, subDocument.GetAllocator());

            PutSettings(entryName, CreatePtr<Settings>(entryName, std::move(subDocument)));
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void SettingsManager::Finalize()
    {
        rapidjson::Document document;
        document.SetObject();

        for (const auto& settingsEntry : _settings)
        {
            auto& entryDocument = settingsEntry.second->GetDocument();
            document.AddMember(rapidjson::GenericStringRef(settingsEntry.first.c_str()), entryDocument.GetObject(), entryDocument.GetAllocator());
        }

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

        if (document.Accept(writer))
        {
            std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
            if (!outputStream.is_open() || !outputStream.good())
            {
                Log::CoreWarn("SettingsManager: failed to open file stream");
                return;
            }

            outputStream << buffer.GetString();
            outputStream.close();
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