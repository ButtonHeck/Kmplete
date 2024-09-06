#pragma once 

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"

#include <rapidjson/document.h>

#include <string>
#include <filesystem>

namespace Kmplete
{
    class JsonReader;
    class JsonWriter;

    class Settings
    {
    public:
        KMP_API Settings(const std::string& name, rapidjson::Document&& document);
        KMP_API Settings(const std::string& name);

        KMP_API bool StartSaveObject(const std::string& objectName = std::string(""));
        KMP_API bool EndSaveObject();

        KMP_API bool StartSaveArray(const std::string& arrayName);
        KMP_API bool EndSaveArray();

        KMP_API bool SaveBool(bool value);
        KMP_API bool SaveBool(const std::string& name, bool value);
        KMP_API bool SaveInt(int value);
        KMP_API bool SaveInt(const std::string& name, int value);
        KMP_API bool SaveUInt(unsigned int value);
        KMP_API bool SaveUInt(const std::string& name, unsigned int value);
        KMP_API bool SaveInt64(int64_t value);
        KMP_API bool SaveInt64(const std::string& name, int64_t value);
        KMP_API bool SaveUInt64(uint64_t value);
        KMP_API bool SaveUInt64(const std::string& name, uint64_t value);
        KMP_API bool SaveDouble(double value);
        KMP_API bool SaveDouble(const std::string& name, double value);
        KMP_API bool SaveString(const std::string& value);
        KMP_API bool SaveString(const std::string& name, const std::string& value);

        KMP_API bool ParseToDocument();
        KMP_NODISCARD KMP_API rapidjson::Document& GetDocument();


        KMP_API bool StartLoadObject(const std::string& objectName);
        KMP_API bool StartLoadObject(int index);
        KMP_API bool EndLoadObject();

        KMP_API int StartLoadArray(const std::string& arrayName);
        KMP_API int StartLoadArray(int index);
        KMP_API bool EndLoadArray();

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
        std::string _name;
        rapidjson::Document _document;
        Ptr<JsonReader> _reader;
        Ptr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}