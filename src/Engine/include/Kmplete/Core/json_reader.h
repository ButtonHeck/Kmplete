#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"

#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace Kmplete
{
    class JsonReader
    {
    public:
        KMP_API explicit JsonReader(rapidjson::Document& document);

        KMP_API bool StartObject(const std::string& objectName);
        KMP_API bool StartObject(int index);
        KMP_API bool EndObject();

        KMP_API int StartArray(const std::string& arrayName);
        KMP_API int StartArray(int index);
        KMP_API bool EndArray();

        KMP_NODISCARD KMP_API bool GetBool(int index, bool defaultValue = false);
        KMP_NODISCARD KMP_API bool GetBool(const std::string& name, bool defaultValue = false);
        KMP_NODISCARD KMP_API int GetInt(int index, int defaultValue = 0);
        KMP_NODISCARD KMP_API int GetInt(const std::string& name, int defaultValue = 0);
        KMP_NODISCARD KMP_API unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_NODISCARD KMP_API unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        KMP_NODISCARD KMP_API int64_t GetInt64(int index, int64_t defaultValue = 0);
        KMP_NODISCARD KMP_API int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        KMP_NODISCARD KMP_API uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        KMP_NODISCARD KMP_API uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        KMP_NODISCARD KMP_API double GetDouble(int index, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API double GetDouble(const std::string& name, double defaultValue = 0.0);
        KMP_NODISCARD KMP_API std::string GetString(int index, const std::string& defaultValue = "");
        KMP_NODISCARD KMP_API std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        KMP_NODISCARD std::string GetCurrentScopeString() const;
        KMP_NODISCARD bool ValidToGetFromArray(int index) const;

    private:
        rapidjson::Document& _document;
        std::vector<std::string> _scope;
        std::string _scopeString;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}