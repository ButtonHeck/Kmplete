#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/settings.h"

#include <rapidjson/document.h>

#include <filesystem>
#include <unordered_map>

namespace Kmplete
{
    class SettingsManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(SettingsManager)

        KMP_API SettingsManager(const std::filesystem::path& filename);
        KMP_API ~SettingsManager() = default;

        KMP_NODISCARD KMP_API bool Initialize();
        KMP_API void Finalize() const;

        KMP_API void PutSettings(const std::string& name, const Ptr<Settings>& settings);
        KMP_NODISCARD KMP_API Ptr<Settings> PutSettings(const std::string& name);
        KMP_NODISCARD KMP_API Ptr<Settings> GetSettings(const std::string& name) const;
        KMP_API void SaveSettings() const;

        KMP_API void SetFilename(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API std::filesystem::path GetFilename() const KMP_NOEXCEPT;

    private:
        KMP_NODISCARD rapidjson::Document AssembleDocument() const;
        void WriteDocument(const rapidjson::Document& document) const;

    private:
        std::filesystem::path _filename;
        std::unordered_map<std::string, Ptr<Settings>> _settings;
    };
    //--------------------------------------------------------------------------
}