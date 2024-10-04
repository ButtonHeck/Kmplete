#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/json_reader.h"
#include "Kmplete/Core/json_writer.h"

#include <rapidjson/document.h>

#include <vector>
#include <filesystem>

namespace Kmplete
{
    class JsonDocument
    {
    public:
        KMP_API JsonDocument();
        KMP_API explicit JsonDocument(rapidjson::Document&& document);
        KMP_API explicit JsonDocument(const std::filesystem::path& filename);
        KMP_API ~JsonDocument();

        KMP_API void SetFilename(const std::filesystem::path& filename);
        KMP_NODISCARD KMP_API const std::filesystem::path& GetFilename() const;

        KMP_NODISCARD KMP_API bool Load(const std::filesystem::path& filename);
        KMP_NODISCARD KMP_API bool Load();
        KMP_NODISCARD KMP_API bool Save(const std::filesystem::path& filename);
        KMP_NODISCARD KMP_API bool Save();

        KMP_NODISCARD KMP_API std::vector<Ptr<JsonDocument>> GetChildren() const;

        // writer
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


        // reader
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
        std::filesystem::path _filename;
        rapidjson::Document _document;
        UPtr<JsonReader> _reader;
        UPtr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}