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

    bool JsonReader::StartObject(const char* objectName)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot start object '{}' - current object is null", objectName);
            return false;
        }

        if (!objectName || *objectName == '\0')
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot start object - object's name is empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot start object '{}' - current object '{}' is not of object type", objectName, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(objectName) || !(*_currentObject)[objectName].IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find member '{}', or the member is not an object type", objectName);
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
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not of object type", _scope.scopeString, index);
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

    int JsonReader::StartArray(const char* arrayName)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot start array '{}' - current object is null", arrayName);
            return 0;
        }

        if (!arrayName || *arrayName == '\0')
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot start array - array's name should not be empty");
            return 0;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot start array '{}' - current object '{}' is not of object type", arrayName, _scope.scopeString);
            return 0;
        }

        if (!_currentObject->HasMember(arrayName) || !(*_currentObject)[arrayName].IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find member '{}', or the member is not an array type", arrayName);
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
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not of array type", _scope.scopeString, index);
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
            KMP_LOG_CORE_ERROR("JsonReader: cannot end array - current object is null");
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot end array - current object '{}' is not of array type", _scope.scopeString);
            return false;
        }

        return EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::GetBool(int index, bool defaultValue /*= false*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsBool())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not a bool", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetBool();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::GetBool(const char* name, bool defaultValue /*= false*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get bool '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsBool())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find bool for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetBool();
    }
    //--------------------------------------------------------------------------

    int JsonReader::GetInt(int index, int defaultValue /*= 0*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsInt())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not an int", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetInt();
    }
    //--------------------------------------------------------------------------

    int JsonReader::GetInt(const char* name, int defaultValue /*= 0*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get int '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsInt())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find int for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetInt();
    }
    //--------------------------------------------------------------------------

    unsigned int JsonReader::GetUInt(int index, unsigned int defaultValue /*= 0*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsUint())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not an unsigned int", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetUint();
    }
    //--------------------------------------------------------------------------

    unsigned int JsonReader::GetUInt(const char* name, unsigned int defaultValue /*= 0*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get unsigned int '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsUint())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find unsigned int for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetUint();
    }
    //--------------------------------------------------------------------------

    int64_t JsonReader::GetInt64(int index, int64_t defaultValue /*= 0*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsInt64())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not an int64", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetInt64();
    }
    //--------------------------------------------------------------------------

    int64_t JsonReader::GetInt64(const char* name, int64_t defaultValue /*= 0*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get int64 '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsInt64())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find int64 for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetInt64();
    }
    //--------------------------------------------------------------------------

    uint64_t JsonReader::GetUInt64(int index, uint64_t defaultValue /*= 0*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsUint64())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not an unsigned int64", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetUint64();
    }
    //--------------------------------------------------------------------------

    uint64_t JsonReader::GetUInt64(const char* name, uint64_t defaultValue /*= 0*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get unsigned int64 '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsUint64())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find unsigned int64 for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetUint64();
    }
    //--------------------------------------------------------------------------

    double JsonReader::GetDouble(int index, double defaultValue /*= 0.0*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsDouble())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not a double", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetDouble();
    }
    //--------------------------------------------------------------------------

    double JsonReader::GetDouble(const char* name, double defaultValue /*= 0.0*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get double '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsDouble())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find double for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetDouble();
    }
    //--------------------------------------------------------------------------

    String JsonReader::GetString(int index, const String& defaultValue /*= ""*/)
    {
        if (!ValidToGetFromArray(index))
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsString())
        {
            KMP_LOG_CORE_ERROR("JsonReader: '{}[{}]' is not a string", _scope.scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetString();
    }
    //--------------------------------------------------------------------------

    String JsonReader::GetString(const char* name, const String& defaultValue /*= ""*/)
    {
        if (!_currentObject || !_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot get string '{}' - current object is null or is not of object type", name);
            return defaultValue;
        }

        if (!_currentObject->HasMember(name) || !(*_currentObject)[name].IsString())
        {
            KMP_LOG_CORE_ERROR("JsonReader: cannot find string for '{}/{}'", _scope.scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name].GetString();
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
            KMP_LOG_CORE_ERROR("JsonReader: current object '{}' is not an array", _scope.scopeString);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()) || index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonReader: invalid index [{}] for '{}'", index, _scope.scopeString);
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}