#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_reader.h"
#include "Kmplete/Json/json_writer.h"
#include "Kmplete/Profile/profiler.h"

#include <rapidjson/fwd.h>
#include <rapidjson/document.h>

#include <vector>

namespace Kmplete
{
    class JsonDocument
    {
#if defined(KMP_PROFILE)
    private:
        UPtr<ProfilerTimer> _constructorProfilerTimer;
#endif

    public:
        KMP_DISABLE_COPY_MOVE(JsonDocument)

        KMP_API JsonDocument();
        KMP_API explicit JsonDocument(rapidjson::Document&& document);
        KMP_API explicit JsonDocument(const Path& filename);
        ~JsonDocument() = default;

        KMP_API void SetFilename(const Path& filename) noexcept;
        KMP_NODISCARD KMP_API const Path& GetFilename() const noexcept;

        KMP_NODISCARD KMP_API bool Load(const Path& filename);
        KMP_NODISCARD KMP_API bool Load();
        KMP_NODISCARD KMP_API bool Save(const Path& filename, bool pretty = true);
        KMP_NODISCARD KMP_API bool Save(bool pretty = true);
        KMP_NODISCARD KMP_API String ToString(bool pretty = true);

        KMP_NODISCARD KMP_API bool HasError() const noexcept;

        KMP_API bool AddChildDocument(const String& name, const JsonDocument& child, bool overwrite = true);
        KMP_NODISCARD KMP_API Vector<std::pair<String, Ptr<JsonDocument>>> GetChildren(bool onlyObjects = true) const;


        KMP_API bool StartSetObject(const char* objectName);
        KMP_API bool StartSetObject(int index);
        KMP_API bool EndSetObject();

        KMP_API bool StartSetArray(const char* arrayName, bool overwrite = true);
        KMP_API bool StartSetArray(int index, bool overwrite = true);
        KMP_API bool EndSetArray();

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


        KMP_API bool StartGetObject(const char* objectName);
        KMP_API bool StartGetObject(int index);
        KMP_API bool EndGetObject();

        KMP_API int StartGetArray(const char* arrayName);
        KMP_API int StartGetArray(int index);
        KMP_API bool EndGetArray();

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
        KMP_NODISCARD bool SaveToFile(const rapidjson::StringBuffer& buffer);

    private:
        Path _filename;
        rapidjson::Document _document;
        bool _error;
        UPtr<JsonReader> _reader;
        UPtr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}