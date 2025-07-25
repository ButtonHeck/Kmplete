#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_scope.h"
#include "Kmplete/Profile/profiler.h"

#include <rapidjson/document.h>

namespace Kmplete
{
    class JsonWriter
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(JsonWriter)

        KMP_API explicit JsonWriter(rapidjson::Document& document);
        ~JsonWriter() = default;

        KMP_API bool StartObject(const char* objectName);
        KMP_API bool StartObject(int index);
        KMP_API bool EndObject();

        KMP_API bool StartArray(const char* arrayName, bool overwrite = true);
        KMP_API bool StartArray(int index, bool overwrite = true);
        KMP_API bool EndArray();

        KMP_API bool SetBool(int index, bool value);
        KMP_API bool SetBool(const char* name, bool value);
        KMP_API bool SetInt(int index, int value);
        KMP_API bool SetInt(const char* name, int value);
        KMP_API bool SetUInt(int index, unsigned int value);
        KMP_API bool SetUInt(const char* name, unsigned int value);
        KMP_API bool SetInt64(int index, int64_t value);
        KMP_API bool SetInt64(const char* name, int64_t value);
        KMP_API bool SetUInt64(int index, uint64_t value);
        KMP_API bool SetUInt64(const char* name, uint64_t value);
        KMP_API bool SetDouble(int index, double value);
        KMP_API bool SetDouble(const char* name, double value);
        KMP_API bool SetString(int index, const String& value);
        KMP_API bool SetString(const char* name, const String& value);

    private:
        rapidjson::Document& _document;
        JsonScope _scope;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}