#pragma once 

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Json/json_document.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    //! Wrapper for a named setting document, currently used only with JSON format,
    //! responsible for managing lifetime of the contained document object and
    //! delegating read/write commands to it.
    //! @see JsonDocument
    class KMP_API SettingsDocument
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(SettingsDocument)

    public:
        SettingsDocument(const String& name, Ptr<JsonDocument> document);
        explicit SettingsDocument(const String& name);
        ~SettingsDocument() = default;

        KMP_NODISCARD const String& GetName() const noexcept;

        bool StartSaveObject(const char* objectName);
        bool StartSaveObject(int index);
        bool EndSaveObject();

        bool StartSaveArray(const char* arrayName, bool overwrite = true);
        bool StartSaveArray(int index, bool overwrite = true);
        bool EndSaveArray();

        bool SaveBool(int index, bool value);
        bool SaveBool(const char* name, bool value);
        bool SaveInt(int index, int value);
        bool SaveInt(const char* name, int value);
        bool SaveUInt(int index, unsigned int value);
        bool SaveUInt(const char* name, unsigned int value);
        bool SaveInt64(int index, Int64 value);
        bool SaveInt64(const char* name, Int64 value);
        bool SaveUInt64(int index, UInt64 value);
        bool SaveUInt64(const char* name, UInt64 value);
        bool SaveDouble(int index, double value);
        bool SaveDouble(const char* name, double value);
        bool SaveString(int index, const String& value);
        bool SaveString(const char* name, const String& value);

        KMP_NODISCARD const JsonDocument& GetDocument() const;


        bool StartLoadObject(const char* objectName);
        bool StartLoadObject(int index);
        bool EndLoadObject();

        int StartLoadArray(const char* arrayName);
        int StartLoadArray(int index);
        bool EndLoadArray();

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
        const String _name;
        Ptr<JsonDocument> _document;
    };
    //--------------------------------------------------------------------------
}