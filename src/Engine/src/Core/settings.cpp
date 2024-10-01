#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/json_reader.h"
#include "Kmplete/Core/json_writer.h"

namespace Kmplete
{
    Settings::Settings(const std::string& name, rapidjson::Document&& document)
        : _name(name)
        , _document(std::move(document))
        , _reader(CreatePtr<JsonReader>(_document))
        , _writer(CreatePtr<JsonWriter>(_document))
    {}
    //--------------------------------------------------------------------------

    Settings::Settings(const std::string& name)
        : _name(name)
        , _reader(CreatePtr<JsonReader>(_document))
        , _writer(CreatePtr<JsonWriter>(_document))
    {}
    //--------------------------------------------------------------------------

    const std::string& Settings::GetName() const
    {
        return _name;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveObject(const std::string& objectName)
    {
        return _writer->StartObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveObject(int index)
    {
        return _writer->StartObject(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSaveObject()
    {
        return _writer->EndObject();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveArray(const std::string& arrayName)
    {
        return _writer->StartArray(arrayName);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveArray(int index)
    {
        return _writer->StartArray(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSaveArray()
    {
        return _writer->EndArray();
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveBool(int index, bool value)
    {
        return _writer->SetBool(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveBool(const std::string& name, bool value)
    {
        return _writer->SetBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(int index, int value)
    {
        return _writer->SetInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(const std::string& name, int value)
    {
        return _writer->SetInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(int index, unsigned int value)
    {
        return _writer->SetUInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(const std::string& name, unsigned int value)
    {
        return _writer->SetUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(int index, int64_t value)
    {
        return _writer->SetInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(const std::string& name, int64_t value)
    {
        return _writer->SetInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(int index, uint64_t value)
    {
        return _writer->SetUInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(const std::string& name, uint64_t value)
    {
        return _writer->SetUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(int index, double value)
    {
        return _writer->SetDouble(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(const std::string& name, double value)
    {
        return _writer->SetDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(int index, const std::string& value)
    {
        return _writer->SetString(index, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(const std::string& name, const std::string& value)
    {
        return _writer->SetString(name, value);
    }
    //--------------------------------------------------------------------------

    rapidjson::Document& Settings::GetDocument()
    {
        return _document;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadObject(const std::string& objectName)
    {
        return _reader->StartObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadObject(int index)
    {
        return _reader->StartObject(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoadObject()
    {
        return _reader->EndObject();
    }
    //--------------------------------------------------------------------------

    int Settings::StartLoadArray(const std::string& arrayName)
    {
        return _reader->StartArray(arrayName);
    }
    //--------------------------------------------------------------------------

    int Settings::StartLoadArray(int index)
    {
        return _reader->StartArray(index);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoadArray()
    {
        return _reader->EndArray();
    }
    //--------------------------------------------------------------------------

    bool Settings::GetBool(int index, bool defaultValue)
    {
        return _reader->GetBool(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool Settings::GetBool(const std::string& name, bool defaultValue)
    {
        return _reader->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int Settings::GetInt(int index, int defaultValue)
    {
        return _reader->GetInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int Settings::GetInt(const std::string& name, int defaultValue)
    {
        return _reader->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int Settings::GetUInt(int index, unsigned int defaultValue)
    {
        return _reader->GetUInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int Settings::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        return _reader->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t Settings::GetInt64(int index, int64_t defaultValue)
    {
        return _reader->GetInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t Settings::GetInt64(const std::string& name, int64_t defaultValue)
    {
        return _reader->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t Settings::GetUInt64(int index, uint64_t defaultValue)
    {
        return _reader->GetUInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t Settings::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        return _reader->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double Settings::GetDouble(int index, double defaultValue)
    {
        return _reader->GetDouble(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    double Settings::GetDouble(const std::string& name, double defaultValue)
    {
        return _reader->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string Settings::GetString(int index, const std::string& defaultValue)
    {
        return _reader->GetString(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string Settings::GetString(const std::string& name, const std::string& defaultValue)
    {
        return _reader->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------     
}