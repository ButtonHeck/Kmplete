#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace Kmplete
{
    class JsonWriter
    {
    public:
        KMP_DISABLE_COPY_MOVE(JsonWriter)

        KMP_API explicit JsonWriter(rapidjson::Document& document);

        KMP_API bool StartObject(const std::string& objectName);
        KMP_API bool StartObject(int index);
        KMP_API bool EndObject();

        KMP_API bool StartArray(const std::string& arrayName);
        KMP_API bool StartArray(int index);
        KMP_API bool EndArray();

        KMP_API bool SaveBool(int index, bool value);
        KMP_API bool SaveBool(const std::string& name, bool value);
        KMP_API bool SaveInt(int index, int value);
        KMP_API bool SaveInt(const std::string& name, int value);
        KMP_API bool SaveUInt(int index, unsigned int value);
        KMP_API bool SaveUInt(const std::string& name, unsigned int value);
        KMP_API bool SaveInt64(int index, int64_t value);
        KMP_API bool SaveInt64(const std::string& name, int64_t value);
        KMP_API bool SaveUInt64(int index, uint64_t value);
        KMP_API bool SaveUInt64(const std::string& name, uint64_t value);
        KMP_API bool SaveDouble(int index, double value);
        KMP_API bool SaveDouble(const std::string& name, double value);
        KMP_API bool SaveString(int index, const std::string& value);
        KMP_API bool SaveString(const std::string& name, const std::string& value);

    private:
        KMP_NODISCARD std::string GetCurrentScopeString() const;

    private:
        rapidjson::Document& _document;
        std::vector<std::string> _scope;
        std::string _scopeString;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}