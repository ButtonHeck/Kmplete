#pragma once 

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"

#include <string>
#include <filesystem>

namespace Kmplete
{
    class JsonReader;
    class JsonWriter;

    class Settings
    {
    public:
        KMP_API explicit Settings(const std::filesystem::path& filename);

        KMP_API KMP_NODISCARD bool Initialize();

        KMP_API bool StartSave();
        KMP_API bool EndSave();

        KMP_API bool StartSaveGroup(const std::string& groupName);
        KMP_API bool EndSaveGroup();

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


        KMP_API bool StartLoadGroup(const std::string& groupName);
        KMP_API bool EndLoadGroup();

        KMP_API int StartLoadArray(const std::string& arrayName);
        KMP_API bool EndLoadArray();

        KMP_API KMP_NODISCARD bool GetBool(int index, bool defaultValue = false);
        KMP_API KMP_NODISCARD bool GetBool(const std::string& name, bool defaultValue = false);
        KMP_API KMP_NODISCARD int GetInt(int index, int defaultValue = 0);
        KMP_API KMP_NODISCARD int GetInt(const std::string& name, int defaultValue = 0);
        KMP_API KMP_NODISCARD unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_API KMP_NODISCARD unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        KMP_API KMP_NODISCARD int64_t GetInt64(int index, int64_t defaultValue = 0);
        KMP_API KMP_NODISCARD int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        KMP_API KMP_NODISCARD uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        KMP_API KMP_NODISCARD uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        KMP_API KMP_NODISCARD double GetDouble(int index, double defaultValue = 0.0);
        KMP_API KMP_NODISCARD double GetDouble(const std::string& name, double defaultValue = 0.0);
        KMP_API KMP_NODISCARD std::string GetString(int index, const std::string& defaultValue = "");
        KMP_API KMP_NODISCARD std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        const std::filesystem::path _filename;
        const Ptr<JsonReader> _reader;
        const Ptr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}