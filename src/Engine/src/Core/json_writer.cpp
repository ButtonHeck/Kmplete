#include "Kmplete/Core/json_writer.h"
#include "Kmplete/Core/log.h"

#include <fstream>

namespace Kmplete
{
    JsonWriter::JsonWriter(rapidjson::Document& document)
        : _document(document)
        , _stringBuffer()
        , _writer(_stringBuffer)
    {}
    //--------------------------------------------------------------------------

    bool JsonWriter::Start()
    {
        return _writer.StartObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::End()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartObject()
    {
        return _writer.StartObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndObject()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartGroup(const std::string& groupName)
    {
        if (groupName.empty())
        {
            Log::CoreWarn("JsonWriter: group name should not be empty!");
            return false;
        }

        if (_writer.Key(groupName.c_str()))
        {
            return _writer.StartObject();
        }

        Log::CoreWarn("JsonWriter: cannot start saving group '{}'", groupName);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndGroup()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(const std::string& arrayName)
    {
        if (_writer.Key(arrayName.c_str()))
        {
            return _writer.StartArray();
        }

        Log::CoreWarn("JsonWriter: cannot save array '{}'", arrayName);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndArray()
    {
        return _writer.EndArray();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveBool(bool value)
    {
        return _writer.Bool(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveBool(const std::string& name, bool value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Bool(value);
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt(int value)
    {
        return _writer.Int(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt(const std::string& name, int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int(value);
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt(unsigned int value)
    {
        return _writer.Uint(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt(const std::string& name, unsigned int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint(value);
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt64(int64_t value)
    {
        return _writer.Int64(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt64(const std::string& name, int64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int64(value);
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt64(uint64_t value)
    {
        return _writer.Uint64(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt64(const std::string& name, uint64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint64(value);
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveDouble(double value)
    {
        return _writer.Double(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveDouble(const std::string& name, double value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Double(value);
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveString(const std::string& value)
    {
        return _writer.String(value.c_str());
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveString(const std::string& name, const std::string& value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.String(value.c_str());
        }

        Log::CoreWarn("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::ToDocument()
    {
        _document.Parse(_stringBuffer.GetString());

        return !_document.HasParseError();
    }
    //--------------------------------------------------------------------------
}