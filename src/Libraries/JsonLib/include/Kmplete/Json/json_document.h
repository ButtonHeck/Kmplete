#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_reader.h"
#include "Kmplete/Json/json_writer.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

#include <rapidjson/fwd.h>
#include <rapidjson/document.h>


namespace Kmplete
{
    //! Wrapper for a JSON document that uses RapidJSON as backend, responsible for storing state of
    //! a document, reading/writing operations, merging several json nodes, saving/loading to/from a file.
    //! Reading and writing operations are delegated to separate JsonReader/JsonWriter objects
    //! @see JsonReader
    //! @see JsonWriter
    class KMP_API JsonDocument
    {
        KMP_LOG_CLASSNAME(JsonDocument)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(JsonDocument)

    public:
        JsonDocument();
        explicit JsonDocument(rapidjson::Document&& document);
        explicit JsonDocument(const Filepath& filepath);
        ~JsonDocument() = default;

        void SetFilepath(const Filepath& filepath) noexcept;
        KMP_NODISCARD const Filepath& GetFilepath() const noexcept;

        KMP_NODISCARD bool Load(const Filepath& filepath);
        KMP_NODISCARD bool Load();
        KMP_NODISCARD bool Save(const Filepath& filepath, bool pretty = true);
        KMP_NODISCARD bool Save(bool pretty = true);
        KMP_NODISCARD String ToString(bool pretty = true);

        KMP_NODISCARD bool HasError() const noexcept;
        KMP_NODISCARD String ErrorDescription() const noexcept;

        bool AddChildDocument(const String& name, const JsonDocument& child, bool overwrite = true);
        KMP_NODISCARD Vector<Pair<String, Ptr<JsonDocument>>> GetChildren(bool onlyObjects = true) const;


        bool StartSetObject(const char* objectName);
        bool StartSetObject(int index);
        bool EndSetObject();

        bool StartSetArray(const char* arrayName, bool overwrite = true);
        bool StartSetArray(int index, bool overwrite = true);
        bool EndSetArray();

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


        bool StartGetObject(const char* objectName);
        bool StartGetObject(int index);
        bool EndGetObject();

        int StartGetArray(const char* arrayName);
        int StartGetArray(int index);
        bool EndGetArray();

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
        KMP_NODISCARD bool _SaveToFile(const rapidjson::StringBuffer& buffer);

    private:
        Filepath _filepath;
        rapidjson::Document _document;
        bool _error;
        UPtr<JsonReader> _reader;
        UPtr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}