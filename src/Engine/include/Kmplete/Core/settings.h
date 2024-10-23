#pragma once 

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Json/json_document.h"

#include <string>

namespace Kmplete
{
    class Settings
    {
    public:
        KMP_DISABLE_COPY_MOVE(Settings)

        KMP_API Settings(const std::string& name, Ptr<JsonDocument> document);
        KMP_API explicit Settings(const std::string& name);
        KMP_API ~Settings() = default;

        KMP_NODISCARD KMP_API const std::string& GetName() const KMP_NOEXCEPT;

        KMP_API bool StartSaveObject(const std::string& objectName);
        KMP_API bool StartSaveObject(int index);
        KMP_API bool EndSaveObject();

        KMP_API bool StartSaveArray(const std::string& arrayName, bool overwrite = true);
        KMP_API bool StartSaveArray(int index, bool overwrite = true);
        KMP_API bool EndSaveArray();

        KMP_API bool SaveBool(int index, bool value);
        KMP_API bool SaveBool(const std::string& name, bool value);
        KMP_API bool SaveInt(int index, int value);
        KMP_API bool SaveInt(const std::string& name, int value);
        KMP_API bool SaveUInt(int index, unsigned int value);
        KMP_API bool SaveUInt(const std::string& name, unsigned int value);
        KMP_API bool SaveInt64(int index, int64_t value);
        KMP_API bool SaveInt64(const std::string& name, int64_t value);
        KMP_API bool SaveUInt64(int index, uint64_t value);
        KMP_API bool SaveUInt64(const std::string& name, uint64_t value);
        KMP_API bool SaveDouble(int index, double value);
        KMP_API bool SaveDouble(const std::string& name, double value);
        KMP_API bool SaveString(int index, const std::string& value);
        KMP_API bool SaveString(const std::string& name, const std::string& value);

        KMP_NODISCARD KMP_API const JsonDocument& GetDocument() const;


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
        const std::string _name;
        Ptr<JsonDocument> _document;
    };
    //--------------------------------------------------------------------------
}