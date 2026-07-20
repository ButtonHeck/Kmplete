#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Profile/profiler_fwd.h"
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
    class KMP_API LocalizationManager
    {
        KMP_LOG_CLASSNAME(LocalizationManager)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(LocalizationManager)

    public:
        using LocaleChangeCallback = Function<void()>;

    public:
        LocalizationManager() noexcept;
        ~LocalizationManager() = default;

        bool SetLocale(const LocaleStr& localeString);
        KMP_NODISCARD const LocaleStr& GetLocale() const noexcept;

        void AddMessagesPath(const String& path);

        bool AddMessagesDomain(const DomainStr& domain);
        bool RemoveMessagesDomain(const DomainStr& domain);

        void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

        TranslationStr Translate(const DomainStr& domain, const SourceStr& source);
        TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count);
        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context);
        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context);

        KMP_NODISCARD const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const;
        KMP_NODISCARD const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                        const SourceStrSID& sourceSidPlural, int count) const;
        KMP_NODISCARD TranslationStr TranslationFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                          const SourceStrSID& sourceSidPlural, int count) const;
        KMP_NODISCARD const TranslationStr& TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                           const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const;
        KMP_NODISCARD TranslationStr TranslationCtxFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                             const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const;
        KMP_NODISCARD const TranslationStr& TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const;
        KMP_NODISCARD const TranslationStr& TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& fallback) const;
        KMP_NODISCARD const TranslationStr& TranslationCtxOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, 
                                                                     const ContextStrSID& contextSid, const TranslationStr& fallback) const;

        KMP_NODISCARD TranslationStr Translation(const DomainStr& domain, const SourceStr& source, const LocaleStr& localeString);
        KMP_NODISCARD TranslationStr Translation(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const LocaleStr& localeString);
        KMP_NODISCARD TranslationStr TranslationCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const LocaleStr& localeString);
        KMP_NODISCARD TranslationStr TranslationCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const LocaleStr& localeString);

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