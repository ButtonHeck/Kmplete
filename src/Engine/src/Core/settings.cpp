#include "Kmplete/Core/settings.h"

namespace Kmplete
{
    Settings::Settings(const std::string& name, Ptr<JsonDocument> document)
        : _name(name)
        , _document(document)
    {}
    //--------------------------------------------------------------------------

    Settings::Settings(const std::string& name)
        : _name(name)
        , _document(new JsonDocument())
    {}
    //--------------------------------------------------------------------------

    const std::string& Settings::GetName() const noexcept
    {
        return _name;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveObject(const std::string& objectName)
    {
        return _document->StartSetObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveObject(int index)
    {
        return _document->StartSetObject(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSaveObject()
    {
        return _document->EndSetObject();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveArray(const std::string& arrayName, bool overwrite)
    {
        return _document->StartSetArray(arrayName, overwrite);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveArray(int index, bool overwrite)
    {
        return _document->StartSetArray(index, overwrite);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSaveArray()
    {
        return _document->EndSetArray();
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveBool(int index, bool value)
    {
        return _document->SetBool(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveBool(const std::string& name, bool value)
    {
        return _document->SetBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(int index, int value)
    {
        return _document->SetInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(const std::string& name, int value)
    {
        return _document->SetInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(int index, unsigned int value)
    {
        return _document->SetUInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(const std::string& name, unsigned int value)
    {
        return _document->SetUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(int index, int64_t value)
    {
        return _document->SetInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(const std::string& name, int64_t value)
    {
        return _document->SetInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(int index, uint64_t value)
    {
        return _document->SetUInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(const std::string& name, uint64_t value)
    {
        return _document->SetUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(int index, double value)
    {
        return _document->SetDouble(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(const std::string& name, double value)
    {
        return _document->SetDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(int index, const std::string& value)
    {
        return _document->SetString(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(const std::string& name, const std::string& value)
    {
        return _document->SetString(name, value);
    }
    //--------------------------------------------------------------------------

    const JsonDocument& Settings::GetDocument() const
    {
        return *_document.get();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadObject(const std::string& objectName)
    {
        return _document->StartGetObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadObject(int index)
    {
        return _document->StartGetObject(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoadObject()
    {
        return _document->EndGetObject();
    }
    //--------------------------------------------------------------------------

    int Settings::StartLoadArray(const std::string& arrayName)
    {
        return _document->StartGetArray(arrayName);
    }
    //--------------------------------------------------------------------------

    int Settings::StartLoadArray(int index)
    {
        return _document->StartGetArray(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoadArray()
    {
        return _document->EndGetArray();
    }
    //--------------------------------------------------------------------------

    bool Settings::GetBool(int index, bool defaultValue)
    {
        return _document->GetBool(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool Settings::GetBool(const std::string& name, bool defaultValue)
    {
        return _document->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int Settings::GetInt(int index, int defaultValue)
    {
        return _document->GetInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int Settings::GetInt(const std::string& name, int defaultValue)
    {
        return _document->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int Settings::GetUInt(int index, unsigned int defaultValue)
    {
        return _document->GetUInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int Settings::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        return _document->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t Settings::GetInt64(int index, int64_t defaultValue)
    {
        return _document->GetInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t Settings::GetInt64(const std::string& name, int64_t defaultValue)
    {
        return _document->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t Settings::GetUInt64(int index, uint64_t defaultValue)
    {
        return _document->GetUInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t Settings::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        return _document->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double Settings::GetDouble(int index, double defaultValue)
    {
        return _document->GetDouble(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    double Settings::GetDouble(const std::string& name, double defaultValue)
    {
        return _document->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string Settings::GetString(int index, const std::string& defaultValue)
    {
        return _document->GetString(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string Settings::GetString(const std::string& name, const std::string& defaultValue)
    {
        return _document->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------     
}