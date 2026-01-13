#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_scope.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

#include <rapidjson/document.h>


namespace Kmplete
{
    //! Json data reader, a node from which data is being read is controlled
    //! by a value pointer whose "address" is stored in a scope variable. 
    //! This class doesn't support indexing like: "int a = document[object][child];"
    //! @see JsonScope
    class JsonReader
    {
        KMP_LOG_CLASSNAME(JsonReader)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(JsonReader)

    public:
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
        KMP_NODISCARD KMP_API Int64 GetInt64(int index, Int64 defaultValue = 0);
        KMP_NODISCARD KMP_API Int64 GetInt64(const char* name, Int64 defaultValue = 0);
        KMP_NODISCARD KMP_API UInt64 GetUInt64(int index, UInt64 defaultValue = 0);
        KMP_NODISCARD KMP_API UInt64 GetUInt64(const char* name, UInt64 defaultValue = 0);
        KMP_NODISCARD KMP_API double GetDouble(int index, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API double GetDouble(const char* name, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API String GetString(int index, const String& defaultValue = "");
        KMP_NODISCARD KMP_API String GetString(const char* name, const String& defaultValue = "");

    private:
        KMP_NODISCARD bool _ValidToGetFromArray(int index) const;

    private:
        rapidjson::Document& _document;
        JsonScope _scope;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}