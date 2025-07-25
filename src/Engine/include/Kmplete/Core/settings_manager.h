#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Profile/profiler.h"

#include <unordered_map>

namespace Kmplete
{
    class SettingsManager
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(SettingsManager)

        KMP_API explicit SettingsManager(const Path& filename);
        ~SettingsManager() = default;

        KMP_NODISCARD KMP_API Nullable<Settings*> PutSettings(const String& name);
        KMP_NODISCARD KMP_API Nullable<Settings*> GetSettings(const String& name) const;

        KMP_API bool LoadSettings();
        KMP_API bool SaveSettings() const;

        KMP_API void SetFilename(const Path& path) noexcept;
        KMP_NODISCARD KMP_API const Path& GetFilename() const noexcept;

    private:
        Path _filename;
        std::unordered_map<String, UPtr<Settings>> _namedSettings;
    };
    //--------------------------------------------------------------------------
}