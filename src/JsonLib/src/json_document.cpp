#include "Kmplete/Json/json_document.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>

#include <fstream>

#if defined (GetObject)
    #pragma push_macro("GetObject")
    #undef GetObject
    #define KMP_UNDEF_GetObject
#endif

namespace Kmplete
{
    JsonDocument::JsonDocument()
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("JsonDocument::JsonDocument()")
          _filename()
        , _document()
        , _error(false)
        , _reader(new JsonReader(_document))
        , _writer(new JsonWriter(_document))
    {
        _document.SetObject();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    JsonDocument::JsonDocument(rapidjson::Document&& document)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("JsonDocument::JsonDocument(rapidjson::Document&&)")
          _filename()
        , _document(std::move(document))
        , _error(_document.HasParseError())
        , _reader(new JsonReader(_document))
        , _writer(new JsonWriter(_document))
    {
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    JsonDocument::JsonDocument(const Filepath& filename)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("JsonDocument::JsonDocument(const Filepath&)")
          _filename(filename)
        , _document()
        , _error(false)
        , _reader(new JsonReader(_document))
        , _writer(new JsonWriter(_document))
    {
        if (!Load(_filename))
        {
            KMP_LOG_ERROR("JsonDocument: creation from '{}' failed", _filename);
        }

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    void JsonDocument::SetFilename(const Filepath& filename) noexcept
    {
        KMP_PROFILE_FUNCTION();

        _filename = filename;

        if (!Filesystem::FilePathIsValid(_filename))
        {
            KMP_LOG_WARN("JsonDocument: invalid filepath was set '{}'", _filename);
        }
    }
    //--------------------------------------------------------------------------

    const Filepath& JsonDocument::GetFilename() const noexcept
    {
        return _filename;
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Load(const Filepath& filename)
    {
        KMP_PROFILE_FUNCTION();

        SetFilename(filename);
        return Load();
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Load()
    {
        KMP_PROFILE_FUNCTION();

        _error = false;
        const auto filenameStr = Filesystem::ToGenericU8String(_filename);
        if (!Filesystem::PathExists(_filename))
        {
            KMP_LOG_WARN("JsonDocument: failed to load due to insufficient path '{}'", filenameStr);
            _error = true;
            return false;
        }

        KMP_LOG_INFO("JsonDocument: loading from '{}'", filenameStr);

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
            KMP_LOG_ERROR("JsonDocument: failed to load from '{}', JSON parsing error '{}'", filenameStr, rapidjson::GetParseError_En(newDocument.GetParseError()));
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

    bool JsonDocument::Save(const Filepath& filename, bool pretty /*= true*/)
    {
        KMP_PROFILE_FUNCTION();

        SetFilename(filename);
        return Save(pretty);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::Save(bool pretty /*= true*/)
    {
        KMP_PROFILE_FUNCTION();

        _error = false;
        rapidjson::StringBuffer buffer;

        if (pretty)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            if (_document.Accept(writer))
            {
                return SaveToFile(buffer);
            }
        }
        else
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (_document.Accept(writer))
            {
                return SaveToFile(buffer);
            }
        }

        KMP_LOG_WARN("JsonDocument: failed to write document in '{}'", _filename);
        _error = true;
        return false;
    }
    //--------------------------------------------------------------------------

    String JsonDocument::ToString(bool pretty /*= true*/)
    {
        KMP_PROFILE_FUNCTION();

        _error = false;
        rapidjson::StringBuffer buffer;

        if (pretty)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

            if (_document.Accept(writer))
            {
                return String(buffer.GetString());
            }
        }
        else
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

            if (_document.Accept(writer))
            {
                return String(buffer.GetString());
            }
        }

        KMP_LOG_WARN("JsonDocument: failed to write document to string");
        _error = true;
        return String("");
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::HasError() const noexcept
    {
        return _error;
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::AddChildDocument(const String& name, const JsonDocument& child, bool overwrite /*= true*/)
    {
        KMP_PROFILE_FUNCTION();

        if (name.empty())
        {
            KMP_LOG_ERROR("JsonDocument: cannot add child document - name is empty");
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
                KMP_LOG_WARN("JsonDocument: already contains member '{}' and overwrite set to false", name);
                return false;
            }
        }

        auto& childDocument = child._document;
        if (!childDocument.IsObject() || childDocument.HasParseError())
        {
            KMP_LOG_ERROR("JsonDocument: cannot add '{}' child document - not an object or has errors", name);
            return false;
        }

        auto& thisAllocator = _document.GetAllocator();
        rapidjson::Value childDeepCopy(childDocument, thisAllocator);
        _document.AddMember(rapidjson::Value(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), thisAllocator).Move(), childDeepCopy, thisAllocator);

        return true;
    }
    //--------------------------------------------------------------------------

    Vector<std::pair<String, Ptr<JsonDocument>>> JsonDocument::GetChildren(bool onlyObjects /*= true*/) const
    {
        KMP_PROFILE_FUNCTION();

        Vector<std::pair<String, Ptr<JsonDocument>>> children;
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

    bool JsonDocument::StartSetObject(const char* objectName)
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

    bool JsonDocument::StartSetArray(const char* arrayName, bool overwrite /*= true*/)
    {
        return _writer->StartArray(arrayName, overwrite);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::StartSetArray(int index, bool overwrite /*= true*/)
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

    bool JsonDocument::SetBool(const char* name, bool value)
    {
        return _writer->SetBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt(int index, int value)
    {
        return _writer->SetInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt(const char* name, int value)
    {
        return _writer->SetInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt(int index, unsigned int value)
    {
        return _writer->SetUInt(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt(const char* name, unsigned int value)
    {
        return _writer->SetUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt64(int index, int64_t value)
    {
        return _writer->SetInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetInt64(const char* name, int64_t value)
    {
        return _writer->SetInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt64(int index, uint64_t value)
    {
        return _writer->SetUInt64(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetUInt64(const char* name, uint64_t value)
    {
        return _writer->SetUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetDouble(int index, double value)
    {
        return _writer->SetDouble(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetDouble(const char* name, double value)
    {
        return _writer->SetDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetString(int index, const String& value)
    {
        return _writer->SetString(index, value);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SetString(const char* name, const String& value)
    {
        return _writer->SetString(name, value);
    }
    //--------------------------------------------------------------------------


    bool JsonDocument::StartGetObject(const char* objectName)
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

    int JsonDocument::StartGetArray(const char* arrayName)
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

    bool JsonDocument::GetBool(int index, bool defaultValue /*= false*/)
    {
        return _reader->GetBool(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::GetBool(const char* name, bool defaultValue /*= false*/)
    {
        return _reader->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int JsonDocument::GetInt(int index, int defaultValue /*= 0*/)
    {
        return _reader->GetInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int JsonDocument::GetInt(const char* name, int defaultValue /*= 0*/)
    {
        return _reader->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int JsonDocument::GetUInt(int index, unsigned int defaultValue /*= 0*/)
    {
        return _reader->GetUInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int JsonDocument::GetUInt(const char* name, unsigned int defaultValue /*= 0*/)
    {
        return _reader->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t JsonDocument::GetInt64(int index, int64_t defaultValue /*= 0*/)
    {
        return _reader->GetInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t JsonDocument::GetInt64(const char* name, int64_t defaultValue /*= 0*/)
    {
        return _reader->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t JsonDocument::GetUInt64(int index, uint64_t defaultValue /*= 0*/)
    {
        return _reader->GetUInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t JsonDocument::GetUInt64(const char* name, uint64_t defaultValue /*= 0*/)
    {
        return _reader->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double JsonDocument::GetDouble(int index, double defaultValue /*= 0.0*/)
    {
        return _reader->GetDouble(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    double JsonDocument::GetDouble(const char* name, double defaultValue /*= 0.0*/)
    {
        return _reader->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    String JsonDocument::GetString(int index, const String& defaultValue /*= ""*/)
    {
        return _reader->GetString(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    String JsonDocument::GetString(const char* name, const String& defaultValue /*= ""*/)
    {
        return _reader->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool JsonDocument::SaveToFile(const rapidjson::StringBuffer& buffer)
    {
        KMP_PROFILE_FUNCTION();

        if (!Filesystem::CreateDirectories(_filename, true))
        {
            KMP_LOG_WARN("JsonDocument: failed to create directories for '{}'", _filename);
            _error = true;
            return false;
        }

        std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            KMP_LOG_WARN("JsonDocument: failed to open file stream for saving in '{}'", _filename);
            _error = true;
            return false;
        }

        outputStream << buffer.GetString();
        outputStream.close();

        KMP_LOG_INFO("JsonDocument: document written successfully in '{}'", _filename);
        _error = false;
        return true;
    }
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_GetObject)
    #pragma pop_macro("GetObject")
    #undef KMP_UNDEF_GetObject
#endif
