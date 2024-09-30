#include "Kmplete/Core/json_writer.h"
#include "Kmplete/Core/log.h"

#include <rapidjson/pointer.h>

#include <algorithm>
#include <numeric>
#include <fstream>

namespace Kmplete
{
    JsonWriter::JsonWriter(rapidjson::Document& document)
        : _document(document)
        , _scope()
        , _scopeString("")
        , _currentObject(rapidjson::Pointer("").Get(_document))
    {
        if (!_currentObject->IsObject())
        {
            _currentObject->SetObject();
        }
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartObject(const std::string& objectName)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (objectName.empty())
        {
            return false;
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot fetch object pointer for '{}'", _scopeString);
            return false;
        }

        _scope.push_back(objectName);
        _scopeString = GetCurrentScopeString();

        if (!_currentObject->HasMember(objectName.c_str()) || !(*_currentObject)[objectName.c_str()].IsObject())
        {
            Log::CoreInfo("JsonWriter: cannot find member '{}', or the member is not an object type", objectName);
            rapidjson::Pointer(_scopeString.c_str()).Create(_document).SetObject();
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartObject(int index)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: current object '{}' is not an array", _scopeString);
            return false;
        }

        _scope.push_back(std::to_string(index));
        _scopeString = GetCurrentScopeString();
        if (index >= static_cast<int>(_currentObject->Size()))
        {
            rapidjson::Pointer(_scopeString.c_str()).Create(_document).SetObject();
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndObject()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            _scopeString = GetCurrentScopeString();
            _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

            return true;
        }

        Log::CoreError("JsonWriter: cannot end save object, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(const std::string& arrayName)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (arrayName.empty())
        {
            Log::CoreWarn("JsonWriter: array name should not be empty!");
            return false;
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot fetch object pointer for '{}'", _scopeString);
            return false;
        }

        if (!_currentObject->IsObject())
        {
            Log::CoreError("JsonWriter: can't start array '{}' from non-object '{}'", arrayName, _scopeString);
            return false;
        }

        _scope.push_back(arrayName);
        _scopeString = GetCurrentScopeString();

        if (!_currentObject->HasMember(arrayName.c_str()) || !(*_currentObject)[arrayName.c_str()].IsArray())
        {
            Log::CoreInfo("JsonWriter: cannot find member '{}', or the member is not an array type", arrayName);
            rapidjson::Pointer(_scopeString.c_str()).Create(_document).SetArray();
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(int index)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: current object '{}' is not an array", _scopeString);
            return false;
        }

        _scope.push_back(std::to_string(index));
        _scopeString = GetCurrentScopeString();

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            rapidjson::Pointer(_scopeString.c_str()).Create(_document).SetArray();
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndArray()
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        return EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveBool(int index, bool value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetBool(value);
        }
        
        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveBool(const std::string& name, bool value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsBool())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetBool(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetBool(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt(int index, int value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetInt(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt(const std::string& name, int value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetInt(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetInt(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt(int index, unsigned int value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetUint(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt(const std::string& name, unsigned int value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetUint(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetUint(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt64(int index, int64_t value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetInt64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt64(const std::string& name, int64_t value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt64())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetInt64(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetInt64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt64(int index, uint64_t value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetUint64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt64(const std::string& name, uint64_t value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint64())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetUint64(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetUint64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveDouble(int index, double value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetDouble(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveDouble(const std::string& name, double value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsDouble())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetDouble(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetDouble(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveString(int index, const std::string& value)
    {
        if (!_currentObject)
        {
            return false;
        }

        if (!_currentObject->IsArray())
        {
            return false;
        }

        const auto size = static_cast<unsigned int>(strlen(value.c_str()));
        if (index >= static_cast<int>(_currentObject->Size()))
        {
            _currentObject->PushBack(rapidjson::Value(value.c_str(), size, _document.GetAllocator()), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetString(value.c_str(), size, _document.GetAllocator());
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveString(const std::string& name, const std::string& value)
    {
        if (!_currentObject)
        {
            return false;
        }

        const auto size = static_cast<unsigned int>(strlen(value.c_str()));
        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsString())
        {
            const auto newScope = _scopeString + "/" + name;
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetString(value.c_str(), size, _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[name.c_str()].SetString(value.c_str(), size, _document.GetAllocator());
        }

        return true;
    }
    //--------------------------------------------------------------------------

    std::string JsonWriter::GetCurrentScopeString() const
    {
        return _scope.empty()
            ? ""
            : std::accumulate(_scope.begin(), _scope.end(), std::string(),
                [](const std::string& a, const std::string& b) {
                    return a + '/' + b;
                });
    }
    //--------------------------------------------------------------------------
}