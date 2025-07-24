#pragma once 

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_document.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    class Settings
    {
#if defined(KMP_PROFILE)
    private:
        UPtr<ProfilerTimer> _constructorProfilerTimer;
#endif

    public:
        KMP_DISABLE_COPY_MOVE(Settings)

        KMP_API Settings(const String& name, Ptr<JsonDocument> document);
        KMP_API explicit Settings(const String& name);
        ~Settings() = default;

        KMP_NODISCARD KMP_API const String& GetName() const noexcept;

        KMP_API bool StartSaveObject(const char* objectName);
        KMP_API bool StartSaveObject(int index);
        KMP_API bool EndSaveObject();

        KMP_API bool StartSaveArray(const char* arrayName, bool overwrite = true);
        KMP_API bool StartSaveArray(int index, bool overwrite = true);
        KMP_API bool EndSaveArray();

        KMP_API bool SaveBool(int index, bool value);
        KMP_API bool SaveBool(const char* name, bool value);
        KMP_API bool SaveInt(int index, int value);
        KMP_API bool SaveInt(const char* name, int value);
        KMP_API bool SaveUInt(int index, unsigned int value);
        KMP_API bool SaveUInt(const char* name, unsigned int value);
        KMP_API bool SaveInt64(int index, int64_t value);
        KMP_API bool SaveInt64(const char* name, int64_t value);
        KMP_API bool SaveUInt64(int index, uint64_t value);
        KMP_API bool SaveUInt64(const char* name, uint64_t value);
        KMP_API bool SaveDouble(int index, double value);
        KMP_API bool SaveDouble(const char* name, double value);
        KMP_API bool SaveString(int index, const String& value);
        KMP_API bool SaveString(const char* name, const String& value);

        KMP_NODISCARD KMP_API const JsonDocument& GetDocument() const;


        KMP_API bool StartLoadObject(const char* objectName);
        KMP_API bool StartLoadObject(int index);
        KMP_API bool EndLoadObject();

        KMP_API int StartLoadArray(const char* arrayName);
        KMP_API int StartLoadArray(int index);
        KMP_API bool EndLoadArray();

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
        const String _name;
        Ptr<JsonDocument> _document;
    };
    //--------------------------------------------------------------------------
}