#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

#include <unordered_map>

namespace Kmplete
{
    class SettingsManager
    {
        KMP_LOG_CLASSNAME(SettingsManager)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(SettingsManager)

        KMP_API explicit SettingsManager(const Filepath& filepath);
        ~SettingsManager() = default;

        KMP_NODISCARD KMP_API Nullable<Settings*> PutSettings(const String& name);
        KMP_NODISCARD KMP_API Nullable<Settings*> GetSettings(const String& name) const;

        KMP_API bool LoadSettings();
        KMP_API bool SaveSettings() const;

        KMP_API void SetFilepath(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API const Filepath& GetFilepath() const noexcept;

    private:
        Filepath _filepath;
        std::unordered_map<String, UPtr<Settings>> _namedSettings;
    };
    //--------------------------------------------------------------------------
}