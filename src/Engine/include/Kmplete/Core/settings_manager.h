#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/pointers.h"

#include <filesystem>
#include <unordered_map>

namespace Kmplete
{
    class SettingsManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(SettingsManager)

        KMP_API explicit SettingsManager(const std::filesystem::path& filename);
        KMP_API ~SettingsManager() = default;

        KMP_NODISCARD KMP_API Settings* const PutSettings(const std::string& name);
        KMP_NODISCARD KMP_API Settings* const GetSettings(const std::string& name) const;

        KMP_API bool LoadSettings();
        KMP_API bool SaveSettings() const;

        KMP_API void SetFilename(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API const std::filesystem::path& GetFilename() const KMP_NOEXCEPT;

    private:
        std::filesystem::path _filename;
        std::unordered_map<std::string, UPtr<Settings>> _settings;
    };
    //--------------------------------------------------------------------------
}