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

        KMP_API bool StartArrayObject(int index);
        KMP_API bool EndArrayObject();

        KMP_API bool StartGroup(const std::string& groupName);
        KMP_API bool EndGroup();

        KMP_API int StartArray(const std::string& arrayName);
        KMP_API bool EndArray();

        KMP_API KMP_NODISCARD bool GetBool(int index, bool defaultValue = false);
        KMP_API KMP_NODISCARD bool GetBool(const std::string& name, bool defaultValue = false);
        KMP_API KMP_NODISCARD int GetInt(int index, int defaultValue = 0);
        KMP_API KMP_NODISCARD int GetInt(const std::string& name, int defaultValue = 0);
        KMP_API KMP_NODISCARD unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        KMP_API KMP_NODISCARD unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        KMP_API KMP_NODISCARD int64_t GetInt64(int index, int64_t defaultValue = 0);
        KMP_API KMP_NODISCARD int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        KMP_API KMP_NODISCARD uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        KMP_API KMP_NODISCARD uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        KMP_API KMP_NODISCARD double GetDouble(int index, double defaultValue = 0.0);
        KMP_API KMP_NODISCARD double GetDouble(const std::string& name, double defaultValue = 0.0);
        KMP_API KMP_NODISCARD std::string GetString(int index, const std::string& defaultValue = "");
        KMP_API KMP_NODISCARD std::string GetString(const std::string& name, const std::string& defaultValue = "");

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