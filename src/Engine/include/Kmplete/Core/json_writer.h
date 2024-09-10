#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <string>

namespace Kmplete
{
    class JsonWriter
    {
    public:
        KMP_DISABLE_COPY_MOVE(JsonWriter)

        KMP_API explicit JsonWriter(rapidjson::Document& document);

        KMP_API bool StartObject(const std::string& objectName = std::string(""));
        KMP_API bool EndObject();

        KMP_API bool StartArray(const std::string& arrayName = std::string(""));
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

        KMP_NODISCARD KMP_API bool ParseToDocument();

    private:
        rapidjson::Document& _document;
        rapidjson::StringBuffer _stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> _writer;
    };
    //--------------------------------------------------------------------------
}