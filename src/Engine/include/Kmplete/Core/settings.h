#pragma once 

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"

#include <string>
#include <filesystem>

namespace Kmplete
{
    class JsonReader;
    class JsonWriter;

    class KMP_API Settings
    {
    public:
        explicit Settings(const std::filesystem::path& name);

        KMP_NODISCARD bool Initialize();

        bool StartSave();
        bool EndSave();

        bool StartSaveGroup(const std::string& groupName);
        bool EndSaveGroup();

        bool StartSaveArray(const std::string& arrayName);
        bool EndSaveArray();

        bool SaveBool(bool value);
        bool SaveBool(const std::string& name, bool value);
        bool SaveInt(int value);
        bool SaveInt(const std::string& name, int value);
        bool SaveUInt(unsigned int value);
        bool SaveUInt(const std::string& name, unsigned int value);
        bool SaveInt64(int64_t value);
        bool SaveInt64(const std::string& name, int64_t value);
        bool SaveUInt64(uint64_t value);
        bool SaveUInt64(const std::string& name, uint64_t value);
        bool SaveDouble(double value);
        bool SaveDouble(const std::string& name, double value);
        bool SaveString(const std::string& value);
        bool SaveString(const std::string& name, const std::string& value);


        bool StartLoadGroup(const std::string& groupName);
        bool EndLoadGroup();

        int StartLoadArray(const std::string& arrayName);
        bool EndLoadArray();

        KMP_NODISCARD bool GetBool(int index, bool defaultValue = false);
        KMP_NODISCARD bool GetBool(const std::string& name, bool defaultValue = false);
        KMP_NODISCARD int GetInt(int index, int defaultValue = 0);
        KMP_NODISCARD int GetInt(const std::string& name, int defaultValue = 0);
        KMP_NODISCARD unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_NODISCARD unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        KMP_NODISCARD int64_t GetInt64(int index, int64_t defaultValue = 0);
        KMP_NODISCARD int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        KMP_NODISCARD uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        KMP_NODISCARD uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        KMP_NODISCARD double GetDouble(int index, double defaultValue = 0.0);
        KMP_NODISCARD double GetDouble(const std::string& name, double defaultValue = 0.0);
        KMP_NODISCARD std::string GetString(int index, const std::string& defaultValue = "");
        KMP_NODISCARD std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        const std::filesystem::path _filename;
        const Ptr<JsonReader> _reader;
        const Ptr<JsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}