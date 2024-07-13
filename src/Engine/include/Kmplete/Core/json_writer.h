#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <string>

namespace Kmplete
{
    class JsonWriter
    {
    public:
        KMP_API explicit JsonWriter(const std::filesystem::path& filename);

        KMP_API bool Start();
        KMP_API bool End();

        KMP_API bool StartObject();
        KMP_API bool EndObject();

        KMP_API bool StartGroup(const std::string& groupName);
        KMP_API bool EndGroup();

        KMP_API bool StartArray(const std::string& arrayName);
        KMP_API bool EndArray();

        KMP_API bool SaveBool(bool value);
        KMP_API bool SaveBool(const std::string& name, bool value);
        KMP_API bool SaveInt(int value);
        KMP_API bool SaveInt(const std::string& name, int value);
        KMP_API bool SaveUInt(unsigned int value);
        KMP_API bool SaveUInt(const std::string& name, unsigned int value);
        KMP_API bool SaveInt64(int64_t value);
        KMP_API bool SaveInt64(const std::string& name, int64_t value);
        KMP_API bool SaveUInt64(uint64_t value);
        KMP_API bool SaveUInt64(const std::string& name, uint64_t value);
        KMP_API bool SaveDouble(double value);
        KMP_API bool SaveDouble(const std::string& name, double value);
        KMP_API bool SaveString(const std::string& value);
        KMP_API bool SaveString(const std::string& name, const std::string& value);

    private:
        const std::filesystem::path _filename;

        rapidjson::StringBuffer _stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> _writer;
    };
    //--------------------------------------------------------------------------
}