#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Json/json_reader.h"
#include "Kmplete/Json/json_writer.h"

#include <rapidjson/fwd.h>
#include <rapidjson/document.h>

#include <vector>

namespace Kmplete
{
    class JsonDocument
    {
    public:
        KMP_DISABLE_COPY_MOVE(JsonDocument)

        KMP_API JsonDocument();
        KMP_API explicit JsonDocument(rapidjson::Document&& document);
        KMP_API explicit JsonDocument(const Path& filename);
        ~JsonDocument() = default;

        KMP_API void SetFilename(const Path& filename) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API const Path& GetFilename() const KMP_NOEXCEPT;

        KMP_NODISCARD KMP_API bool Load(const Path& filename);
        KMP_NODISCARD KMP_API bool Load();
        KMP_NODISCARD KMP_API bool Save(const Path& filename, bool pretty = true);
        KMP_NODISCARD KMP_API bool Save(bool pretty = true);
        KMP_NODISCARD KMP_API std::string ToString(bool pretty = true);

        KMP_NODISCARD KMP_API bool HasError() const KMP_NOEXCEPT;

        KMP_API bool AddChildDocument(const std::string& name, const JsonDocument& child, bool overwrite = true);
        KMP_NODISCARD KMP_API std::vector<std::pair<std::string, Ptr<JsonDocument>>> GetChildren(bool onlyObjects = true) const;


        KMP_API bool StartSetObject(const std::string& objectName);
        KMP_API bool StartSetObject(int index);
        KMP_API bool EndSetObject();

        KMP_API bool StartSetArray(const std::string& arrayName, bool overwrite = true);
        KMP_API bool StartSetArray(int index, bool overwrite = true);
        KMP_API bool EndSetArray();

        KMP_API bool SetBool(int index, bool value);
        KMP_API bool SetBool(const std::string& name, bool value);
        KMP_API bool SetInt(int index, int value);
        KMP_API bool SetInt(const std::string& name, int value);
        KMP_API bool SetUInt(int index, unsigned int value);
        KMP_API bool SetUInt(const std::string& name, unsigned int value);
        KMP_API bool SetInt64(int index, int64_t value);
        KMP_API bool SetInt64(const std::string& name, int64_t value);
        KMP_API bool SetUInt64(int index, uint64_t value);
        KMP_API bool SetUInt64(const std::string& name, uint64_t value);
        KMP_API bool SetDouble(int index, double value);
        KMP_API bool SetDouble(const std::string& name, double value);
        KMP_API bool SetString(int index, const std::string& value);
        KMP_API bool SetString(const std::string& name, const std::string& value);


        KMP_API bool StartGetObject(const std::string& objectName);
        KMP_API bool StartGetObject(int index);
        KMP_API bool EndGetObject();

        KMP_API int StartGetArray(const std::string& arrayName);
        KMP_API int StartGetArray(int index);
        KMP_API bool EndGetArray();

        KMP_NODISCARD KMP_API bool GetBool(int index, bool defaultValue = false);
        KMP_NODISCARD KMP_API bool GetBool(const std::string& name, bool defaultValue = false);
        KMP_NODISCARD KMP_API int GetInt(int index, int defaultValue = 0);
        KMP_NODISCARD KMP_API int GetInt(const std::string& name, int defaultValue = 0);
        KMP_NODISCARD KMP_API unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_NODISCARD KMP_API unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        KMP_NODISCARD KMP_API int64_t GetInt64(int index, int64_t defaultValue = 0);
        KMP_NODISCARD KMP_API int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        KMP_NODISCARD KMP_API uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        KMP_NODISCARD KMP_API uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        KMP_NODISCARD KMP_API double GetDouble(int index, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API double GetDouble(const std::string& name, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API std::string GetString(int index, const std::string& defaultValue = "");
        KMP_NODISCARD KMP_API std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        KMP_NODISCARD bool SaveToFile(const rapidjson::StringBuffer& buffer, const std::string& filenameStr);

    private:
        Path _filename;
        rapidjson::Document _document;
        bool _error;
        UPtr<JsonReader> _reader;
        UPtr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}