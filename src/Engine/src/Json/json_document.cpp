#include "Kmplete/Json/json_document.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>

#include <fstream>

#ifdef GetObject
#pragma push_macro("GetObject")
#undef GetObject
#define KMP_UNDEF_GetObject
#endif

namespace Kmplete
{
    JsonDocument::JsonDocument()
        : _filename()
        , _document()
        , _error(false)
        , _reader(new JsonReader(_document))
        , _writer(new JsonWriter(_document))
    {
        _document.SetObject();
    }
    //--------------------------------------------------------------------------

    JsonDocument::JsonDocument(rapidjson::Document&& document)
        : _filename()
        , _document(std::move(document))
        , _error(_document.HasParseError())
        , _reader(new JsonReader(_document))
        , _writer(new JsonWriter(_document))
    {}
    //--------------------------------------------------------------------------

    JsonDocument::JsonDocument(const std::filesystem::path& filename)
        : _filename(filename)
        , _document()
        , _error(false)
        , _reader(new JsonReader(_document))
        , _writer(new JsonWriter(_document))
    {
        if (!Load(_filename))
        {
            Log::CoreError("JsonDocument: creation from '{}' failed", Filesystem::ToGenericU8String(_filename));
        }
    }
    //--------------------------------------------------------------------------

    void JsonDocument::SetFilename(const std::filesystem::path& filename) KMP_NOEXCEPT
    {
        _filename = filename;

        if (!Filesystem::FilePathIsValid(_filename))
        {
            Log::CoreWarn("JsonDocument: invalid filepath was set '{}'", Filesystem::ToGenericU8String(_filename));
        }
    }
    //--------------------------------------------------------------------------

    const std::filesystem::path& JsonDocument::GetFilename() const KMP_NOEXCEPT
    {
        return _filename;
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Load(const std::filesystem::path& filename)
    {
        SetFilename(filename);
        return Load();
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Load()
    {
        _error = false;
        const auto filenameStr = Filesystem::ToGenericU8String(_filename);
        if (!Filesystem::PathExists(_filename))
        {
            Log::CoreWarn("JsonDocument: failed to load due to insufficient path '{}'", filenameStr);
            _error = true;
            return false;
        }

        Log::CoreInfo("JsonDocument: loading from '{}'", filenameStr);

        std::ifstream inputStream(_filename);
        if (!inputStream.is_open() || !inputStream.good())
        {
            _error = true;
            return false;
        }

        rapidjson::Document newDocument;
        rapidjson::IStreamWrapper jsonStream(inputStream);
        newDocument.ParseStream(jsonStream);
        inputStream.close();

        if (newDocument.HasParseError())
        {
            Log::CoreError("JsonDocument: failed to load from '{}', JSON parsing error '{}'", filenameStr, rapidjson::GetParseError_En(newDocument.GetParseError()));
            _error = true;
            return false;
        }

        _document.Swap(newDocument);
        _reader.reset(new JsonReader(_document));
        _writer.reset(new JsonWriter(_document));

        _error = false;
        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Save(const std::filesystem::path& filename, bool pretty)
    {
        SetFilename(filename);
        return Save(pretty);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Save(bool pretty)
    {
        _error = false;
        rapidjson::StringBuffer buffer;

        const auto filenameStr = Filesystem::ToGenericU8String(_filename);

        if (pretty)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            if (_document.Accept(writer))
            {
                return SaveToFile(buffer, filenameStr);
            }
        }
        else
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (_document.Accept(writer))
            {
                return SaveToFile(buffer, filenameStr);
            }
        }

        Log::CoreWarn("JsonDocument: failed to write document in '{}'", filenameStr);
        _error = true;
        return false;
    }
    //--------------------------------------------------------------------------

    std::string JsonDocument::ToString(bool pretty)
    {
        _error = false;
        rapidjson::StringBuffer buffer;

        if (pretty)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

            if (_document.Accept(writer))
            {
                return std::string(buffer.GetString());
            }
        }
        else
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

            if (_document.Accept(writer))
            {
                return std::string(buffer.GetString());
            }
        }

        Log::CoreWarn("JsonDocument: failed to write document to string");
        _error = true;
        return std::string("");
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::HasError() const KMP_NOEXCEPT
    {
        return _error;
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::AddChildDocument(const std::string& name, const JsonDocument& child, bool overwrite)
    {
        if (name.empty())
        {
            Log::CoreError("JsonDocument: cannot add child document - name is empty");
            return false;
        }

        if (_document.HasMember(name.c_str()))
        {
            if (overwrite)
            {
                _document.RemoveMember(name.c_str());
            }
            else
            {
                Log::CoreWarn("JsonDocument: already contains member '{}' and overwrite set to false", name);
                return false;
            }
        }

        auto& childDocument = child._document;
        if (!childDocument.IsObject() || childDocument.HasParseError())
        {
            Log::CoreError("JsonDocument: cannot add '{}' child document - not an object or has errors", name);
            return false;
        }

        auto& thisAllocator = _document.GetAllocator();
        rapidjson::Value childDeepCopy(childDocument, thisAllocator);
        _document.AddMember(rapidjson::Value(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), thisAllocator).Move(), childDeepCopy, thisAllocator);

        return true;
    }
    //--------------------------------------------------------------------------

    std::vector<std::pair<std::string, Ptr<JsonDocument>>> JsonDocument::GetChildren(bool onlyObjects) const
    {
        std::vector<std::pair<std::string, Ptr<JsonDocument>>> children;
        children.reserve(_document.MemberCount());
        for (auto child = _document.MemberBegin(); child != _document.MemberEnd(); child++)
        {
            if (onlyObjects && !child->value.IsObject())
            {
                continue;
            }

            const auto childName = child->name.GetString();
            rapidjson::Document childDocument;
            childDocument.CopyFrom(child->value, childDocument.GetAllocator());

            children.emplace_back(childName, CreatePtr<JsonDocument>(std::move(childDocument)));
        }

        return children;
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::StartSetObject(const std::string& objectName)
    {
        return _writer->StartObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::StartSetObject(int index)
    {
        return _writer->StartObject(index);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::EndSetObject()
    {
        return _writer->EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::StartSetArray(const std::string& arrayName, bool overwrite)
    {
        return _writer->StartArray(arrayName, overwrite);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::StartSetArray(int index, bool overwrite)
    {
        return _writer->StartArray(index, overwrite);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::EndSetArray()
    {
        return _writer->EndArray();
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetBool(int index, bool value)
    {
        return _writer->SetBool(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetBool(const std::string& name, bool value)
    {
        return _writer->SetBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt(int index, int value)
    {
        return _writer->SetInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt(const std::string& name, int value)
    {
        return _writer->SetInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt(int index, unsigned int value)
    {
        return _writer->SetUInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt(const std::string& name, unsigned int value)
    {
        return _writer->SetUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt64(int index, int64_t value)
    {
        return _writer->SetInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt64(const std::string& name, int64_t value)
    {
        return _writer->SetInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt64(int index, uint64_t value)
    {
        return _writer->SetUInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt64(const std::string& name, uint64_t value)
    {
        return _writer->SetUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetDouble(int index, double value)
    {
        return _writer->SetDouble(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetDouble(const std::string& name, double value)
    {
        return _writer->SetDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetString(int index, const std::string& value)
    {
        return _writer->SetString(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetString(const std::string& name, const std::string& value)
    {
        return _writer->SetString(name, value);
    }
    //--------------------------------------------------------------------------


    bool JsonDocument::StartGetObject(const std::string& objectName)
    {
        return _reader->StartObject(objectName);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::StartGetObject(int index)
    {
        return _reader->StartObject(index);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::EndGetObject()
    {
        return _reader->EndObject();
    }
    //--------------------------------------------------------------------------

    int JsonDocument::StartGetArray(const std::string& arrayName)
    {
        return _reader->StartArray(arrayName);
    }
    //--------------------------------------------------------------------------

    int JsonDocument::StartGetArray(int index)
    {
        return _reader->StartArray(index);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::EndGetArray()
    {
        return _reader->EndArray();
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::GetBool(int index, bool defaultValue)
    {
        return _reader->GetBool(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::GetBool(const std::string& name, bool defaultValue)
    {
        return _reader->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int JsonDocument::GetInt(int index, int defaultValue)
    {
        return _reader->GetInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int JsonDocument::GetInt(const std::string& name, int defaultValue)
    {
        return _reader->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int JsonDocument::GetUInt(int index, unsigned int defaultValue)
    {
        return _reader->GetUInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int JsonDocument::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        return _reader->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t JsonDocument::GetInt64(int index, int64_t defaultValue)
    {
        return _reader->GetInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t JsonDocument::GetInt64(const std::string& name, int64_t defaultValue)
    {
        return _reader->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t JsonDocument::GetUInt64(int index, uint64_t defaultValue)
    {
        return _reader->GetUInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t JsonDocument::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        return _reader->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double JsonDocument::GetDouble(int index, double defaultValue)
    {
        return _reader->GetDouble(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    double JsonDocument::GetDouble(const std::string& name, double defaultValue)
    {
        return _reader->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string JsonDocument::GetString(int index, const std::string& defaultValue)
    {
        return _reader->GetString(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string JsonDocument::GetString(const std::string& name, const std::string& defaultValue)
    {
        return _reader->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SaveToFile(const rapidjson::StringBuffer& buffer, const std::string& filenameStr)
    {
        if (!Filesystem::CreateDirectories(_filename, true))
        {
            Log::CoreWarn("JsonDocument: failed to create directories for '{}'", filenameStr);
            _error = true;
            return false;
        }

        std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            Log::CoreWarn("JsonDocument: failed to open file stream for saving in '{}'", filenameStr);
            _error = true;
            return false;
        }

        outputStream << buffer.GetString();
        outputStream.close();

        Log::CoreInfo("JsonDocument: document written successfully in '{}'", filenameStr);
        _error = false;
        return true;
    }
    //--------------------------------------------------------------------------
}

#ifdef KMP_UNDEF_GetObject
#pragma pop_macro("GetObject")
#undef KMP_UNDEF_GetObject
#endif
