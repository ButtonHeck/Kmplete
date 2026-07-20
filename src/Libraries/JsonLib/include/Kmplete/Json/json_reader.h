#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_scope.h"
#include "Kmplete/Profile/profiler_fwd.h"
#include "Kmplete/Log/log_class_macro.h"

#include <rapidjson/document.h>


namespace Kmplete
{
    //! Json data reader, a node from which data is being read is controlled
    //! by a value pointer whose "address" is stored in a scope variable. 
    //! This class doesn't support indexing like: "int a = document[object][child];"
    //! @see JsonScope
    class KMP_API JsonReader
    {
        KMP_LOG_CLASSNAME(JsonReader)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(JsonReader)

    public:
        explicit JsonReader(rapidjson::Document& document);
        ~JsonReader() = default;

        bool StartObject(const char* objectName);
        bool StartObject(int index);
        bool EndObject();

        int StartArray(const char* arrayName);
        int StartArray(int index);
        bool EndArray();

        KMP_NODISCARD bool GetBool(int index, bool defaultValue = false);
        KMP_NODISCARD bool GetBool(const char* name, bool defaultValue = false);
        KMP_NODISCARD int GetInt(int index, int defaultValue = 0);
        KMP_NODISCARD int GetInt(const char* name, int defaultValue = 0);
        KMP_NODISCARD unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_NODISCARD unsigned int GetUInt(const char* name, unsigned int defaultValue = 0);
        KMP_NODISCARD Int64 GetInt64(int index, Int64 defaultValue = 0);
        KMP_NODISCARD Int64 GetInt64(const char* name, Int64 defaultValue = 0);
        KMP_NODISCARD UInt64 GetUInt64(int index, UInt64 defaultValue = 0);
        KMP_NODISCARD UInt64 GetUInt64(const char* name, UInt64 defaultValue = 0);
        KMP_NODISCARD double GetDouble(int index, double defaultValue = 0.0);
        KMP_NODISCARD double GetDouble(const char* name, double defaultValue = 0.0);
        KMP_NODISCARD String GetString(int index, const String& defaultValue = "");
        KMP_NODISCARD String GetString(const char* name, const String& defaultValue = "");

    private:
        KMP_NODISCARD bool _ValidToGetFromArray(int index) const;

    private:
        rapidjson::Document& _document;
        JsonScope _scope;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}