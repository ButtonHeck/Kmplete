#include "Kmplete/Json/json_reader.h"
#include "Kmplete/Core/log.h"

#include <rapidjson/pointer.h>

namespace Kmplete
{
    JsonReader::JsonReader(rapidjson::Document& document)
        : _document(document)
        , _scope()
        , _currentObject(rapidjson::Pointer("").Get(_document))
    {}
    //--------------------------------------------------------------------------

    bool JsonReader::StartObject(const std::string& objectName)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonReader: cannot start object '{}' - current object is null", objectName);
            return false;
        }

        if (objectName.empty())
        {
            Log::CoreError("JsonReader: cannot start object - object's name is empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot start object '{}' - current object '{}' is not of object type", objectName, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(objectName.c_str()) || !(*_currentObject)[objectName.c_str()].IsObject())
        {
            Log::CoreError("JsonReader: cannot find member '{}', or the member is not an object type", objectName);
            return false;
        }

        _scope.Push(objectName);
        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::StartObject(int index)
    {
        if (!ValidToGetFromArray(index))
        {
            return false;
        }

        if (!(*_currentObject)[index].IsObject())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not of object type", _scope.scopeString, index);
            return false;
        }

        _scope.Push(std::to_string(index));
        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::EndObject()
    {
        if (_scope.Pop())
        {
            _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);
            return true;
        }

        return false;
    }
    //--------------------------------------------------------------------------

    int JsonReader::StartArray(const std::string& arrayName)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonReader: cannot start array '{}' - current object is null", arrayName);
            return 0;
        }

        if (arrayName.empty())
        {
            Log::CoreError("JsonReader: cannot start array - array's name should not be empty");
            return 0;
        }

        if (!_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot start array '{}' - current object '{}' is not of object type", arrayName, _scope.scopeString);
            return 0;
        }

        if (!_currentObject->HasMember(arrayName.c_str()) || !(*_currentObject)[arrayName.c_str()].IsArray())
        {
            Log::CoreError("JsonReader: cannot find member '{}', or the member is not an array type", arrayName);
            return 0;
        }

        _scope.Push(arrayName);
        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return (*_currentObject).GetArray().Size();
    }
    //--------------------------------------------------------------------------

    int JsonReader::StartArray(int index)
    {
        if (!ValidToGetFromArray(index))
        {
            return 0;
        }

        if (!(*_currentObject)[index].IsArray())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not of array type", _scope.scopeString, index);
            return 0;
        }

        _scope.Push(std::to_string(index));
        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return (*_currentObject).GetArray().Size();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::EndArray()
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonReader: cannot end array - current object is null");
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonReader: cannot end array - current object '{}' is not of array type", _scope.scopeString);
            return false;
        }

        return EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::GetBool(int index, bool defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsBool())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not a bool", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetBool();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::GetBool(const std::string& name, bool defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get bool '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsBool())
        {
            Log::CoreError("JsonReader: cannot find bool for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetBool();
    }
    //--------------------------------------------------------------------------

    int JsonReader::GetInt(int index, int defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsInt())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not an int", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetInt();
    }
    //--------------------------------------------------------------------------

    int JsonReader::GetInt(const std::string& name, int defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get int '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt())
        {
            Log::CoreError("JsonReader: cannot find int for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetInt();
    }
    //--------------------------------------------------------------------------

    unsigned int JsonReader::GetUInt(int index, unsigned int defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsUint())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not an unsigned int", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetUint();
    }
    //--------------------------------------------------------------------------

    unsigned int JsonReader::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get unsigned int '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint())
        {
            Log::CoreError("JsonReader: cannot find unsigned int for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetUint();
    }
    //--------------------------------------------------------------------------

    int64_t JsonReader::GetInt64(int index, int64_t defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsInt64())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not an int64", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetInt64();
    }
    //--------------------------------------------------------------------------

    int64_t JsonReader::GetInt64(const std::string& name, int64_t defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get int64 '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt64())
        {
            Log::CoreError("JsonReader: cannot find int64 for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetInt64();
    }
    //--------------------------------------------------------------------------

    uint64_t JsonReader::GetUInt64(int index, uint64_t defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsUint64())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not an unsigned int64", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetUint64();
    }
    //--------------------------------------------------------------------------

    uint64_t JsonReader::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get unsigned int64 '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint64())
        {
            Log::CoreError("JsonReader: cannot find unsigned int64 for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetUint64();
    }
    //--------------------------------------------------------------------------

    double JsonReader::GetDouble(int index, double defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsDouble())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not a double", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetDouble();
    }
    //--------------------------------------------------------------------------

    double JsonReader::GetDouble(const std::string& name, double defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get double '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsDouble())
        {
            Log::CoreError("JsonReader: cannot find double for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetDouble();
    }
    //--------------------------------------------------------------------------

    std::string JsonReader::GetString(int index, const std::string& defaultValue)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsString())
        {
            Log::CoreError("JsonReader: '{}[{}]' is not a string", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetString();
    }
    //--------------------------------------------------------------------------

    std::string JsonReader::GetString(const std::string& name, const std::string& defaultValue)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            Log::CoreError("JsonReader: cannot get string '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsString())
        {
            Log::CoreError("JsonReader: cannot find string for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetString();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::ValidToGetFromArray(int index) const
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonReader: current object '{}' is not an array", _scope.scopeString);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()) || index < 0)
        {
            Log::CoreError("JsonReader: invalid index [{}] for '{}'", index, _scope.scopeString);
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}