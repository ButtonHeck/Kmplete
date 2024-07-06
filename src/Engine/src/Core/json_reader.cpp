#include "Kmplete/Core/json_reader.h"
#include "Kmplete/Core/log.h"

#include <rapidjson/pointer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <algorithm>
#include <numeric>
#include <fstream>

namespace Kmplete
{
    JsonReader::JsonReader(const std::filesystem::path& path)
        : _path(path)
        , _document()
        , _scope()
        , _scopeString("")
        , _currentObject(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool JsonReader::Start()
    {
        KMPLETE_CORE_LOG_INFO("JsonReader: loading from '{}'", Filesystem::ToU8String(_path));

        if (!Filesystem::PathExists(_path))
        {
            KMPLETE_CORE_LOG_WARN("JsonReader: insufficient path");
            return false;
        }

        std::ifstream inputStream(_path);
        if (!inputStream.is_open() || !inputStream.good())
        {
            return false;
        }

        rapidjson::IStreamWrapper jsonStream(inputStream);
        _document.ParseStream(jsonStream);
        inputStream.close();

        if (_document.HasParseError())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: JSON parsing error '{}'", rapidjson::GetParseError_En(_document.GetParseError()));
            return false;
        }

        _currentObject = rapidjson::Pointer("").Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::End()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::StartArrayObject(int index)
    {
        if (!ValidToGetFromArray(index))
        {
            return false;
        }

        if (!(*_currentObject)[index].IsObject())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not an array object", _scopeString, index);
            return false;
        }

        _scope.push_back(std::to_string(index));
        _scopeString = GetCurrentScopeString();
        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::EndArrayObject()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            _scopeString = GetCurrentScopeString();
            _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

            return true;
        }

        KMPLETE_CORE_LOG_ERROR("JsonReader: cannot end load array object, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::StartGroup(const std::string& groupName)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (groupName.empty())
        {
            KMPLETE_CORE_LOG_WARN("JsonReader: group name should not be empty!");
            return false;
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
        if (!_currentObject)
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot fetch object pointer for '{}'", _scopeString);
            return false;
        }

        if (!_currentObject->HasMember(groupName.c_str()) || !(*_currentObject)[groupName.c_str()].IsObject())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find member '{}', or the member is not an object type", groupName);
            return false;
        }

        _scope.push_back(groupName);
        _scopeString = GetCurrentScopeString();
        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonReader::EndGroup()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            _scopeString = GetCurrentScopeString();
            _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

            return true;
        }

        KMPLETE_CORE_LOG_ERROR("JsonReader: cannot end load group, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    int JsonReader::StartArray(const std::string& arrayName)
    {
        if (!_currentObject)
        {
            return 0;
        }

        if (arrayName.empty())
        {
            KMPLETE_CORE_LOG_WARN("JsonReader: array name should not be empty!");
            return 0;
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
        if (!_currentObject)
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot fetch object pointer for '{}'", _scopeString);
            return 0;
        }

        if (!_currentObject->HasMember(arrayName.c_str()) || !(*_currentObject)[arrayName.c_str()].IsArray())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find member '{}', or the member is not an array type", arrayName);
            return 0;
        }

        _scope.push_back(arrayName);
        _scopeString = GetCurrentScopeString();
        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return (*_currentObject).GetArray().Size();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::EndArray()
    {
        return EndGroup();
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not a bool", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetBool();
    }
    //--------------------------------------------------------------------------

    bool JsonReader::GetBool(const std::string& name, bool defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsBool())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find bool for '{}/{}'", _scopeString, name);
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not an int", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetInt();
    }
    //--------------------------------------------------------------------------

    int JsonReader::GetInt(const std::string& name, int defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find int for '{}/{}'", _scopeString, name);
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not an uint", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetUint();
    }
    //--------------------------------------------------------------------------

    unsigned int JsonReader::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find uint for '{}/{}'", _scopeString, name);
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not an int64", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetInt64();
    }
    //--------------------------------------------------------------------------

    int64_t JsonReader::GetInt64(const std::string& name, int64_t defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt64())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find int64 for '{}/{}'", _scopeString, name);
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not an uint64", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetUint64();
    }
    //--------------------------------------------------------------------------

    uint64_t JsonReader::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint64())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find uint64 for '{}/{}'", _scopeString, name);
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not a double", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetDouble();
    }
    //--------------------------------------------------------------------------

    double JsonReader::GetDouble(const std::string& name, double defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsDouble())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find double for '{}/{}'", _scopeString, name);
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: '{}[{}]' is not a string", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetString();
    }
    //--------------------------------------------------------------------------

    std::string JsonReader::GetString(const std::string& name, const std::string& defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsString())
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: cannot find string for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetString();
    }
    //--------------------------------------------------------------------------

    std::string JsonReader::GetCurrentScopeString() const
    {
        return _scope.empty()
            ? ""
            : std::accumulate(_scope.begin(), _scope.end(), std::string(),
                [](const std::string& a, const std::string& b) {
                    return a + '/' + b;
                });
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
            KMPLETE_CORE_LOG_ERROR("JsonReader: current object '{}' is not an array", _scopeString);
            return false;
        }

        if (index >= _currentObject->Size() || index < 0)
        {
            KMPLETE_CORE_LOG_ERROR("JsonReader: invalid index [{}] for '{}'", index, _scopeString);
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}