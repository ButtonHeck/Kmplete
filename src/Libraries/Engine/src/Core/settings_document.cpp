#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    SettingsDocument::SettingsDocument(const String& name, Ptr<JsonDocument> document)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _name(name)
        , _document(document)
    {
        KMP_ASSERT(_document);
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    SettingsDocument::SettingsDocument(const String& name)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _name(name)
        , _document(new JsonDocument())
    {
        KMP_ASSERT(_document);
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    const String& SettingsDocument::GetName() const noexcept
    {
        return _name;
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::StartSaveObject(const char* objectName)
    {
        KMP_ASSERT(_document);

        return _document->StartSetObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::StartSaveObject(int index)
    {
        KMP_ASSERT(_document);

        return _document->StartSetObject(index);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::EndSaveObject()
    {
        KMP_ASSERT(_document);

        return _document->EndSetObject();
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::StartSaveArray(const char* arrayName, bool overwrite /*= true*/)
    {
        KMP_ASSERT(_document);

        return _document->StartSetArray(arrayName, overwrite);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::StartSaveArray(int index, bool overwrite /*= true*/)
    {
        KMP_ASSERT(_document);

        return _document->StartSetArray(index, overwrite);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::EndSaveArray()
    {
        KMP_ASSERT(_document);

        return _document->EndSetArray();
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveBool(int index, bool value)
    {
        KMP_ASSERT(_document);

        return _document->SetBool(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveBool(const char* name, bool value)
    {
        KMP_ASSERT(_document);

        return _document->SetBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveInt(int index, int value)
    {
        KMP_ASSERT(_document);

        return _document->SetInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveInt(const char* name, int value)
    {
        KMP_ASSERT(_document);

        return _document->SetInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveUInt(int index, unsigned int value)
    {
        KMP_ASSERT(_document);

        return _document->SetUInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveUInt(const char* name, unsigned int value)
    {
        KMP_ASSERT(_document);

        return _document->SetUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveInt64(int index, Int64 value)
    {
        KMP_ASSERT(_document);

        return _document->SetInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveInt64(const char* name, Int64 value)
    {
        KMP_ASSERT(_document);

        return _document->SetInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveUInt64(int index, UInt64 value)
    {
        KMP_ASSERT(_document);

        return _document->SetUInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveUInt64(const char* name, UInt64 value)
    {
        KMP_ASSERT(_document);

        return _document->SetUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveDouble(int index, double value)
    {
        KMP_ASSERT(_document);

        return _document->SetDouble(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveDouble(const char* name, double value)
    {
        KMP_ASSERT(_document);

        return _document->SetDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveString(int index, const String& value)
    {
        KMP_ASSERT(_document);

        return _document->SetString(index, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::SaveString(const char* name, const String& value)
    {
        KMP_ASSERT(_document);

        return _document->SetString(name, value);
    }
    //--------------------------------------------------------------------------

    const JsonDocument& SettingsDocument::GetDocument() const
    {
        KMP_ASSERT(_document);

        return *_document.get();
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::StartLoadObject(const char* objectName)
    {
        KMP_ASSERT(_document);

        return _document->StartGetObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::StartLoadObject(int index)
    {
        KMP_ASSERT(_document);

        return _document->StartGetObject(index);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::EndLoadObject()
    {
        KMP_ASSERT(_document);

        return _document->EndGetObject();
    }
    //--------------------------------------------------------------------------

    int SettingsDocument::StartLoadArray(const char* arrayName)
    {
        KMP_ASSERT(_document);

        return _document->StartGetArray(arrayName);
    }
    //--------------------------------------------------------------------------

    int SettingsDocument::StartLoadArray(int index)
    {
        KMP_ASSERT(_document);

        return _document->StartGetArray(index);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::EndLoadArray()
    {
        KMP_ASSERT(_document);

        return _document->EndGetArray();
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::GetBool(int index, bool defaultValue /*= false*/)
    {
        KMP_ASSERT(_document);

        return _document->GetBool(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool SettingsDocument::GetBool(const char* name, bool defaultValue /*= false*/)
    {
        KMP_ASSERT(_document);

        return _document->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int SettingsDocument::GetInt(int index, int defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int SettingsDocument::GetInt(const char* name, int defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsDocument::GetUInt(int index, unsigned int defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetUInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsDocument::GetUInt(const char* name, unsigned int defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    Int64 SettingsDocument::GetInt64(int index, Int64 defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    Int64 SettingsDocument::GetInt64(const char* name, Int64 defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    UInt64 SettingsDocument::GetUInt64(int index, UInt64 defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetUInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    UInt64 SettingsDocument::GetUInt64(const char* name, UInt64 defaultValue /*= 0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double SettingsDocument::GetDouble(int index, double defaultValue /*= 0.0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetDouble(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    double SettingsDocument::GetDouble(const char* name, double defaultValue /*= 0.0*/)
    {
        KMP_ASSERT(_document);

        return _document->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    String SettingsDocument::GetString(int index, const String& defaultValue /*= ""*/)
    {
        KMP_ASSERT(_document);

        return _document->GetString(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    String SettingsDocument::GetString(const char* name, const String& defaultValue /*= ""*/)
    {
        KMP_ASSERT(_document);

        return _document->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------     
}