#include "Kmplete/Core/json_writer.h"
#include "Kmplete/Core/log.h"

#include <rapidjson/pointer.h>

#include <algorithm>
#include <numeric>

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
            Log::CoreError("JsonWriter: cannot start object '{}' - current object is null", objectName);
            return false;
        }

        if (objectName.empty())
        {
            Log::CoreError("JsonWriter: cannot start object - object's name is empty");
            return false;
        }

        PushScope(objectName);

        if (!_currentObject->HasMember(objectName.c_str()) || !(*_currentObject)[objectName.c_str()].IsObject())
        {
            Log::CoreDebug("JsonWriter: creating new object '{}' in '{}'", objectName, _scopeString);
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
            Log::CoreError("JsonWriter: cannot start object '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot start object '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot start object '{}' - negative index", index);
            return false;
        }

        PushScope(std::to_string(index));

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new object '{}' in '{}'", index, _scopeString);
            rapidjson::Pointer(_scopeString.c_str()).Create(_document).SetObject();
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndObject()
    {
        if (PopScope())
        {
            _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
            return true;
        }

        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(const std::string& arrayName)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot start array '{}' - current object is null", arrayName);
            return false;
        }

        if (arrayName.empty())
        {
            Log::CoreError("JsonWriter: cannot start array - array's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            Log::CoreError("JsonWriter: can't start array '{}' from non-object '{}'", arrayName, _scopeString);
            return false;
        }

        PushScope(arrayName);

        if (!_currentObject->HasMember(arrayName.c_str()) || !(*_currentObject)[arrayName.c_str()].IsArray())
        {
            Log::CoreDebug("JsonWriter: creating new array '{}' in '{}'", arrayName, _scopeString);
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
            Log::CoreError("JsonWriter: cannot start array '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot start array '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot start array '{}' - negative index", index);
            return false;
        }

        PushScope(std::to_string(index));

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new array '{}' in '{}'", index, _scopeString);
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
            Log::CoreError("JsonWriter: cannot end array - current object is null");
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot end array - current object '{}' is not of array type", _scopeString);
            return false;
        }

        return EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetBool(int index, bool value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set bool '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set bool '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set bool '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new bool '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetBool(value);
        }
        
        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetBool(const std::string& name, bool value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set bool '{}' - current object is null", name);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsBool())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new bool '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetBool(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetBool(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetInt(int index, int value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set int '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set int '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set int '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new int '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetInt(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetInt(const std::string& name, int value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set int '{}' - current object is null", name);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new int '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetInt(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetInt(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetUInt(int index, unsigned int value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set unsigned int '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set unsigned int '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set unsigned int '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new unsigned int '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetUint(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetUInt(const std::string& name, unsigned int value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set unsigned int '{}' - current object is null", name);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new unsigned int '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetUint(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetUint(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetInt64(int index, int64_t value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set int64 '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set int64 '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set int64 '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new int64 '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetInt64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetInt64(const std::string& name, int64_t value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set int64 '{}' - current object is null", name);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt64())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new int64 '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetInt64(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetInt64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetUInt64(int index, uint64_t value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set unsigned int64 '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set unsigned int64 '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set unsigned int64 '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new unsigned int64 '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetUint64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetUInt64(const std::string& name, uint64_t value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set unsigned int64 '{}' - current object is null", name);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint64())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new unsigned int64 '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetUint64(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetUint64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetDouble(int index, double value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set double '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set double '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set double '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new double '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetDouble(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetDouble(const std::string& name, double value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set double '{}' - current object is null", name);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsDouble())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new double '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetDouble(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetDouble(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetString(int index, const std::string& value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set string '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            Log::CoreError("JsonWriter: cannot set string '{}' - current object '{}' is not of array type", index, _scopeString);
            return false;
        }

        if (index < 0)
        {
            Log::CoreError("JsonWriter: cannot set string '{}' - negative index", index);
            return false;
        }

        const auto size = static_cast<unsigned int>(strlen(value.c_str()));
        if (index >= static_cast<int>(_currentObject->Size()))
        {
            Log::CoreDebug("JsonWriter: creating new string '{}' in '{}'", index, _scopeString);
            _currentObject->PushBack(rapidjson::Value(value.c_str(), size, _document.GetAllocator()), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetString(value.c_str(), size, _document.GetAllocator());
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetString(const std::string& name, const std::string& value)
    {
        if (!_currentObject)
        {
            Log::CoreError("JsonWriter: cannot set string '{}' - current object is null", name);
            return false;
        }

        const auto size = static_cast<unsigned int>(strlen(value.c_str()));
        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsString())
        {
            const auto newScope = _scopeString + "/" + name;
            Log::CoreDebug("JsonWriter: creating new string '{}' in '{}'", name, _scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetString(value.c_str(), size, _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[name.c_str()].SetString(value.c_str(), size, _document.GetAllocator());
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void JsonWriter::PushScope(const std::string& entry)
    {
        _scope.push_back(entry);
        _scopeString = GetCurrentScopeString();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::PopScope()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            _scopeString = GetCurrentScopeString();
            return true;
        }
        
        Log::CoreError("JsonWriter: cannot pop from empty scope");
        return false;
    }
    //--------------------------------------------------------------------------

    std::string JsonWriter::GetCurrentScopeString() const
    {
        if (_scope.empty())
        {
            return "";
        }

        return std::accumulate(_scope.begin(), _scope.end(), std::string(),
                [](const std::string& a, const std::string& b) {
                    return a + '/' + b;
                });
    }
    //--------------------------------------------------------------------------
}