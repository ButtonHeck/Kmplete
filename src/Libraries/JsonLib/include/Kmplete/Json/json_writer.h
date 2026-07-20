#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_scope.h"
#include "Kmplete/Profile/profiler_fwd.h"
#include "Kmplete/Log/log_class_macro.h"

#include <rapidjson/document.h>


namespace Kmplete
{
    //! Json data writer, a node to which data is being written is controlled
    //! by a value pointer whose "address" is stored in a scope variable. 
    //! This class doesn't support indexing like: "document[object][child] = 123;"
    //! @see JsonScope
    class KMP_API JsonWriter
    {
        KMP_LOG_CLASSNAME(JsonWriter)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(JsonWriter)

    public:
        explicit JsonWriter(rapidjson::Document& document);
        ~JsonWriter() = default;

        bool StartObject(const char* objectName);
        bool StartObject(int index);
        bool EndObject();

        bool StartArray(const char* arrayName, bool overwrite = true);
        bool StartArray(int index, bool overwrite = true);
        bool EndArray();

        bool SetBool(int index, bool value);
        bool SetBool(const char* name, bool value);
        bool SetInt(int index, int value);
        bool SetInt(const char* name, int value);
        bool SetUInt(int index, unsigned int value);
        bool SetUInt(const char* name, unsigned int value);
        bool SetInt64(int index, Int64 value);
        bool SetInt64(const char* name, Int64 value);
        bool SetUInt64(int index, UInt64 value);
        bool SetUInt64(const char* name, UInt64 value);
        bool SetDouble(int index, double value);
        bool SetDouble(const char* name, double value);
        bool SetString(int index, const String& value);
        bool SetString(const char* name, const String& value);

    private:
        rapidjson::Document& _document;
        JsonScope _scope;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}