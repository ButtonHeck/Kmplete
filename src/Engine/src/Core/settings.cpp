#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/json_reader.h"
#include "Kmplete/Core/json_writer.h"
#include "Kmplete/Core/filesystem.h"

namespace Kmplete
{
    Settings::Settings(const std::filesystem::path& name)
        : _filename(name)
        , _reader(CreatePtr<JsonReader>(_filename))
        , _writer(CreatePtr<JsonWriter>(_filename))
    {}
    //--------------------------------------------------------------------------

    bool Settings::Initialize()
    {
        return _reader->Initialize();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSave()
    {
        return _writer->Start();
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSave()
    {
        return _writer->End();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveGroup(const std::string& groupName)
    {
        return _writer->StartGroup(groupName);
    }
    //--------------------------------------------------------------------------    

    bool Settings::EndSaveGroup()
    {
        return _writer->EndGroup();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveArray(const std::string& arrayName)
    {
        return _writer->StartArray(arrayName);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSaveArray()
    {
        return _writer->EndArray();
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveBool(bool value)
    {
        return _writer->SaveBool(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveBool(const std::string& name, bool value)
    {
        return _writer->SaveBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(int value)
    {
        return _writer->SaveInt(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(const std::string& name, int value)
    {
        return _writer->SaveInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(unsigned int value)
    {
        return _writer->SaveUInt(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(const std::string& name, unsigned int value)
    {
        return _writer->SaveUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(int64_t value)
    {
        return _writer->SaveInt64(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(const std::string& name, int64_t value)
    {
        return _writer->SaveInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(uint64_t value)
    {
        return _writer->SaveUInt64(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(const std::string& name, uint64_t value)
    {
        return _writer->SaveUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(double value)
    {
        return _writer->SaveDouble(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(const std::string& name, double value)
    {
        return _writer->SaveDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(const std::string& value)
    {
        return _writer->SaveString(value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(const std::string& name, const std::string& value)
    {
        return _writer->SaveString(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadGroup(const std::string& groupName)
    {
        return _reader->StartGroup(groupName);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoadGroup()
    {
        return _reader->EndGroup();
    }
    //--------------------------------------------------------------------------

    int Settings::StartLoadArray(const std::string& arrayName)
    {
        return _reader->StartArray(arrayName);
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