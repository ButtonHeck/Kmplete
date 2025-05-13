#include "Kmplete/Json/json_writer.h"
#include "Kmplete/Core/log.h"

#include <rapidjson/pointer.h>

namespace Kmplete
{
    JsonWriter::JsonWriter(rapidjson::Document& document)
        : _document(document)
        , _scope()
        , _currentObject(rapidjson::Pointer("").Get(_document))
    {
        if (!_currentObject->IsObject())
        {
            _currentObject->SetObject();
        }
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartObject(const String& objectName)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start object '{}' - current object is null", objectName);
            return false;
        }

        if (objectName.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start object - object's name is empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start object '{}' - current object '{}' is not of object type", objectName, _scope.scopeString);
            return false;
        }

        _scope.Push(objectName);

        if (!_currentObject->HasMember(objectName.c_str()) || !(*_currentObject)[objectName.c_str()].IsObject())
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new object '{}' in '{}'", objectName, _scope.scopeString);
            rapidjson::Pointer(_scope.scopeString.c_str()).Create(_document).SetObject();
        }

        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartObject(int index)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start object '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start object '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start object '{}' - negative index", index);
            return false;
        }

        _scope.Push(std::to_string(index));

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new object '{}' in '{}'", index, _scope.scopeString);
            rapidjson::Pointer(_scope.scopeString.c_str()).Create(_document).SetObject();
        }

        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndObject()
    {
        if (_scope.Pop())
        {
            _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);
            return true;
        }

        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(const String& arrayName, bool overwrite)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start array '{}' - current object is null", arrayName);
            return false;
        }

        if (arrayName.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start array - array's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start array '{}' - current object '{}' is not of object type", arrayName, _scope.scopeString);
            return false;
        }

        _scope.Push(arrayName);

        if (!_currentObject->HasMember(arrayName.c_str()) || !(*_currentObject)[arrayName.c_str()].IsArray() || overwrite)
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new array '{}' in '{}'", arrayName, _scope.scopeString);
            rapidjson::Pointer(_scope.scopeString.c_str()).Create(_document).SetArray();
        }

        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(int index, bool overwrite)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start array '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start array '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot start array '{}' - negative index", index);
            return false;
        }

        _scope.Push(std::to_string(index));

        if (index >= static_cast<int>(_currentObject->Size()) || overwrite)
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new array '{}' in '{}'", index, _scope.scopeString);
            rapidjson::Pointer(_scope.scopeString.c_str()).Create(_document).SetArray();
        }

        _currentObject = rapidjson::Pointer(_scope.scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndArray()
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot end array - current object is null");
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot end array - current object '{}' is not of array type", _scope.scopeString);
            return false;
        }

        return EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetBool(int index, bool value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set bool '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set bool '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set bool '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new bool '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetBool(value);
        }
        
        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetBool(const String& name, bool value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set bool '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set bool - bool's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set bool '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsBool())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new bool '{}' in '{}'", name, _scope.scopeString);
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
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new int '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetInt(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetInt(const String& name, int value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int - int's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new int '{}' in '{}'", name, _scope.scopeString);
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
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new unsigned int '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetUint(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetUInt(const String& name, unsigned int value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int - unsigned int's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new unsigned int '{}' in '{}'", name, _scope.scopeString);
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
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int64 '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int64 '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int64 '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new int64 '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetInt64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetInt64(const String& name, int64_t value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int64 '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int64 - int64's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set int64 '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt64())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new int64 '{}' in '{}'", name, _scope.scopeString);
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
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int64 '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int64 '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int64 '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new unsigned int64 '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetUint64(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetUInt64(const String& name, uint64_t value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int64 '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int64 - unsigned int64's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set unsigned int64 '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint64())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new unsigned int64 '{}' in '{}'", name, _scope.scopeString);
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
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set double '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set double '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set double '{}' - negative index", index);
            return false;
        }

        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new double '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetDouble(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetDouble(const String& name, double value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set double '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set double - double's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set double '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsDouble())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new double '{}' in '{}'", name, _scope.scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetDouble(value);
        }
        else
        {
            (*_currentObject)[name.c_str()].SetDouble(value);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetString(int index, const String& value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set string '{}' - current object is null", index);
            return false;
        }

        if (!_currentObject->IsArray())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set string '{}' - current object '{}' is not of array type", index, _scope.scopeString);
            return false;
        }

        if (index < 0)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set string '{}' - negative index", index);
            return false;
        }

        const auto size = static_cast<rapidjson::SizeType>(value.length());
        if (index >= static_cast<int>(_currentObject->Size()))
        {
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new string '{}' in '{}'", index, _scope.scopeString);
            _currentObject->PushBack(rapidjson::Value(value.c_str(), size, _document.GetAllocator()), _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[index].SetString(value.c_str(), size, _document.GetAllocator());
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SetString(const String& name, const String& value)
    {
        if (!_currentObject)
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set string '{}' - current object is null", name);
            return false;
        }

        if (name.empty())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set string - string's name should not be empty");
            return false;
        }

        if (!_currentObject->IsObject())
        {
            KMP_LOG_CORE_ERROR("JsonWriter: cannot set string '{}' - current object '{}' is not of object type", name, _scope.scopeString);
            return false;
        }

        const auto size = static_cast<rapidjson::SizeType>(value.length());
        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsString())
        {
            const auto newScope = _scope.scopeString + "/" + name;
            KMP_LOG_CORE_DEBUG("JsonWriter: creating new string '{}' in '{}'", name, _scope.scopeString);
            rapidjson::Pointer(newScope.c_str()).Create(_document).SetString(value.c_str(), size, _document.GetAllocator());
        }
        else
        {
            (*_currentObject)[name.c_str()].SetString(value.c_str(), size, _document.GetAllocator());
        }

        return true;
    }
    //--------------------------------------------------------------------------
}