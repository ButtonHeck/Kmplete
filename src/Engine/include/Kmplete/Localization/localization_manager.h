#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Profile/profiler.h"

#include <boost/locale.hpp>

#include <vector>
#include <functional>

namespace Kmplete
{
    class Settings;

    class LocalizationManager
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        using LocaleChangeCallback = std::function<void()>;

    public:
        KMP_DISABLE_COPY_MOVE(LocalizationManager)

        KMP_API LocalizationManager() noexcept;
        ~LocalizationManager() = default;

        KMP_API bool SetLocale(const LocaleStr& localeString);
        KMP_NODISCARD KMP_API const LocaleStr& GetLocale() const noexcept;

        KMP_API void AddMessagesPath(const String& path);

        KMP_API bool AddMessagesDomain(const DomainStr& domain);
        KMP_API bool RemoveMessagesDomain(const DomainStr& domain);

        KMP_API void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

        void SaveSettings(Settings& settings) const;
        void LoadSettings(Settings& settings);

        KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& source);
        KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count);
        KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context);
        KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context);

        KMP_NODISCARD KMP_API const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const;
        KMP_NODISCARD KMP_API const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                                const SourceStrSID& sourceSidPlural, int count) const;
        KMP_NODISCARD KMP_API TranslationStr TranslationFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                                  const SourceStrSID& sourceSidPlural, int count) const;
        KMP_NODISCARD KMP_API const TranslationStr& TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                                   const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const;
        KMP_NODISCARD KMP_API TranslationStr TranslationCtxFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                                     const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const;
        KMP_NODISCARD KMP_API const TranslationStr& TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const;
        KMP_NODISCARD KMP_API const TranslationStr& TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& fallback) const;
        KMP_NODISCARD KMP_API const TranslationStr& TranslationCtxOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, 
                                                                             const ContextStrSID& contextSid, const TranslationStr& fallback) const;

        KMP_NODISCARD KMP_API TranslationStr Translation(const DomainStr& domain, const SourceStr& source, const LocaleStr& localeString);
        KMP_NODISCARD KMP_API TranslationStr Translation(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const LocaleStr& localeString);
        KMP_NODISCARD KMP_API TranslationStr TranslationCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const LocaleStr& localeString);
        KMP_NODISCARD KMP_API TranslationStr TranslationCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const LocaleStr& localeString);

    private:
        void ImbueLocale() const;
        void NotifyLocaleListeners() const;

    private:
        boost::locale::generator _localeGenerator;
        UPtr<LocalizationLibrary> _library;
        LocaleStr _currentLocale;
        Vector<LocaleChangeCallback> _localeChangedCallbacks;
    };
    //--------------------------------------------------------------------------
}