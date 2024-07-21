#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/settings.h"

#include <filesystem>
#include <unordered_map>

namespace Kmplete
{
    class SettingsManager
    {
    public:
        KMP_API SettingsManager(const std::filesystem::path& filename);
        KMP_API ~SettingsManager();

        KMP_API bool Initialize();
        KMP_API void Finalize();

        void PutSettings(const std::string& name, const Ptr<Settings>& settings);
        Ptr<Settings> GetSettings(const std::string& name) const;

    private:
        const std::filesystem::path _filename;
        std::unordered_map<std::string, Ptr<Settings>> _settings;
    };
    //--------------------------------------------------------------------------
}