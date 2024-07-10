#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <string>

namespace Kmplete
{
    class KMP_API JsonWriter
    {
    public:
        explicit JsonWriter(const std::filesystem::path& path);

        bool Start();
        bool End();

        bool StartObject();
        bool EndObject();

        bool StartGroup(const std::string& groupName);
        bool EndGroup();

        bool StartArray(const std::string& arrayName);
        bool EndArray();

        bool SaveBool(bool value);
        bool SaveBool(const std::string& name, bool value);
        bool SaveInt(int value);
        bool SaveInt(const std::string& name, int value);
        bool SaveUInt(unsigned int value);
        bool SaveUInt(const std::string& name, unsigned int value);
        bool SaveInt64(int64_t value);
        bool SaveInt64(const std::string& name, int64_t value);
        bool SaveUInt64(uint64_t value);
        bool SaveUInt64(const std::string& name, uint64_t value);
        bool SaveDouble(double value);
        bool SaveDouble(const std::string& name, double value);
        bool SaveString(const std::string& value);
        bool SaveString(const std::string& name, const std::string& value);

    private:
        const std::filesystem::path _path;

        rapidjson::StringBuffer _stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> _writer;
    };
    //--------------------------------------------------------------------------
}