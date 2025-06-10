#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types.h"
#include "Kmplete/Json/json_scope.h"

#include <rapidjson/document.h>

namespace Kmplete
{
    class JsonReader
    {
    public:
        KMP_DISABLE_COPY_MOVE(JsonReader)

        KMP_API explicit JsonReader(rapidjson::Document& document);
        ~JsonReader() = default;

        KMP_API bool StartObject(const char* objectName);
        KMP_API bool StartObject(int index);
        KMP_API bool EndObject();

        KMP_API int StartArray(const char* arrayName);
        KMP_API int StartArray(int index);
        KMP_API bool EndArray();

        KMP_NODISCARD KMP_API bool GetBool(int index, bool defaultValue = false);
        KMP_NODISCARD KMP_API bool GetBool(const char* name, bool defaultValue = false);
        KMP_NODISCARD KMP_API int GetInt(int index, int defaultValue = 0);
        KMP_NODISCARD KMP_API int GetInt(const char* name, int defaultValue = 0);
        KMP_NODISCARD KMP_API unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_NODISCARD KMP_API unsigned int GetUInt(const char* name, unsigned int defaultValue = 0);
        KMP_NODISCARD KMP_API int64_t GetInt64(int index, int64_t defaultValue = 0);
        KMP_NODISCARD KMP_API int64_t GetInt64(const char* name, int64_t defaultValue = 0);
        KMP_NODISCARD KMP_API uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        KMP_NODISCARD KMP_API uint64_t GetUInt64(const char* name, uint64_t defaultValue = 0);
        KMP_NODISCARD KMP_API double GetDouble(int index, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API double GetDouble(const char* name, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API String GetString(int index, const String& defaultValue = "");
        KMP_NODISCARD KMP_API String GetString(const char* name, const String& defaultValue = "");

    private:
        KMP_NODISCARD bool ValidToGetFromArray(int index) const;

    private:
        rapidjson::Document& _document;
        JsonScope _scope;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}