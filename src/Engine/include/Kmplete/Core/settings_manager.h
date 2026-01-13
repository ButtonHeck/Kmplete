#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! Manager of settings document objects, responsible for managing lifetime of contained documents,
    //! creating/deleting document objects, merging, saving and loading documents from/to a file.
    //! @see SettingsDocument
    class SettingsManager
    {
        KMP_LOG_CLASSNAME(SettingsManager)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(SettingsManager)

    public:
        KMP_API explicit SettingsManager(const Filepath& filepath);
        ~SettingsManager() = default;

        KMP_NODISCARD KMP_API OptionalRef<SettingsDocument> PutSettingsDocument(const String& name);
        KMP_NODISCARD KMP_API OptionalRef<SettingsDocument> GetSettingsDocument(const String& name) const;

        KMP_API bool LoadSettings();
        KMP_API bool SaveSettings() const;

        KMP_API void SetFilepath(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API const Filepath& GetFilepath() const noexcept;

    private:
        Filepath _filepath;
        HashMap<String, UPtr<SettingsDocument>> _namedSettingsDocuments;
    };
    //--------------------------------------------------------------------------
}