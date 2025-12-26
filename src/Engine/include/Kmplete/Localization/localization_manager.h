#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

#include <boost/locale.hpp>


namespace Kmplete
{
    class SettingsDocument;


    //! Manager for localization of the application, responsible for handling locale settings,
    //! creating locales, adding/removing translations from files (domains). Uses Boost.Locale library as
    //! the localization backend. Client application should add a callback(s) to this manager in order
    //! to be notified when locale changes.
    //! "Translate[...]" functions names SHOULD MATCH parameters of gettext utility
    //! programs in order to successfully update and compile translations - thus if any renaming occurs, make sure
    //! to rename those parameters as well.
    //! Translations may either be fetched from pre-filled library object or can be "directly" translated using
    //! free functions from localization_translator.h
    //! @see localization_translator.h
    //! @see localization_base.h
    //! @see LocalizationLibrary
    class LocalizationManager
    {
        KMP_LOG_CLASSNAME(LocalizationManager)

    public:
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        using LocaleChangeCallback = Function<void()>;

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

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

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
        void _ImbueLocale() const;
        void _NotifyLocaleListeners() const;

    private:
        boost::locale::generator _localeGenerator;
        UPtr<LocalizationLibrary> _library;
        LocaleStr _currentLocale;
        Vector<LocaleChangeCallback> _localeChangedCallbacks;
    };
    //--------------------------------------------------------------------------
}