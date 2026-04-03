#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_translator.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <iostream>


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "LocalizationManager";
    static constexpr auto SettingsLocaleStr = "Locale";


    LocalizationManager::LocalizationManager() noexcept
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _localeGenerator()
        , _library(CreateUPtr<LocalizationLibrary>())
        , _currentLocale(std::locale().name())
    {
        SetLocale(LocaleEnUTF8Keyword);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::SetLocale(const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportant);

        if (_currentLocale == localeString)
        {
            KMP_LOG_INFO("locale '{}' already set", localeString);
            return false;
        }

        try
        {
            const auto testLocale = localeString.empty() ? std::locale::classic() : std::locale(localeString);
            _currentLocale = testLocale.name();
            std::setlocale(LC_ALL, _currentLocale.c_str());

            _library->SetLocale(_currentLocale);

            _ImbueLocale();
            _NotifyLocaleListeners();

            KMP_LOG_INFO("set locale '{}'", _currentLocale);
            return true;
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_ERROR("cannot set locale '{}' - {}", localeString, e.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    const LocaleStr& LocalizationManager::GetLocale() const noexcept
    {
        return _currentLocale;
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const String& path)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportant);

        _localeGenerator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::AddMessagesDomain(const DomainStr& domain)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportant);

        _localeGenerator.add_messages_domain(domain);
        _ImbueLocale();

        return _library->AddDictionary(domain);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::RemoveMessagesDomain(const DomainStr& domain)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportant);

        return _library->RemoveDictionary(domain);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

        _localeChangedCallbacks.push_back(callback);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportant);

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveString(SettingsLocaleStr, _currentLocale);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportant);

        settings.StartLoadObject(SettingsEntryName);
        const auto localeStr = settings.GetString(SettingsLocaleStr, LocaleEnUTF8Keyword);
        SetLocale(localeStr);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translate(const DomainStr& domain, const SourceStr& source)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::Translate(domain, source);
        const auto domainSid = ToStringID(KMP_SID_PARAM(domain));
        const auto sourceSid = ToStringID(KMP_SID_PARAM(source));
        _library->Add(domainSid, sourceSid, translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::Translate(domain, sourceSingular, sourcePlural, count);
        const auto domainSid = ToStringID(KMP_SID_PARAM(domain));
        const auto sourceSingularSid = ToStringID(KMP_SID_PARAM(sourceSingular));
        const auto sourcePluralSid = ToStringID(KMP_SID_PARAM(sourcePlural));
        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        _library->Add(domainSid, sourceSingularSid, sourcePluralSid, pluralityDispatcher.GetPluralityForm(count), translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::TranslateCtx(domain, source, context);
        const auto domainSid = ToStringID(KMP_SID_PARAM(domain));
        const auto sourceSid = ToStringID(KMP_SID_PARAM(source));
        const auto contextSid = ToStringID(KMP_SID_PARAM(context));
        _library->Add(domainSid, sourceSid, contextSid, translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, 
                                                     int count, const ContextStr& context)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::TranslateCtx(domain, sourceSingular, sourcePlural, count, context);
        const auto domainSid = ToStringID(KMP_SID_PARAM(domain));
        const auto sourceSingularSid = ToStringID(KMP_SID_PARAM(sourceSingular));
        const auto sourcePluralSid = ToStringID(KMP_SID_PARAM(sourcePlural));
        const auto contextSid = ToStringID(KMP_SID_PARAM(context));
        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        _library->Add(domainSid, sourceSingularSid, sourcePluralSid, pluralityDispatcher.GetPluralityForm(count), contextSid, translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        return _library->Get(domainSid, sourceSid);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                           const SourceStrSID& sourceSidPlural, int count) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        return _library->Get(domainSid, sourceSidSingular, sourceSidPlural, pluralityDispatcher.GetPluralityForm(count));
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                             const SourceStrSID& sourceSidPlural, int count) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        return Localization::Format(Translation(domainSid, sourceSidSingular, sourceSidPlural, count), count);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                              const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        return _library->Get(domainSid, sourceSidSingular, sourceSidPlural, pluralityDispatcher.GetPluralityForm(count), contextSid);
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationCtxFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                                const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        return Localization::Format(TranslationCtx(domainSid, sourceSidSingular, sourceSidPlural, count, contextSid), count);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        return _library->Get(domainSid, sourceSid, contextSid);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& fallback) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto& translation = Translation(domainSid, sourceSid);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationCtxOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, 
                                                                        const ContextStrSID& contextSid, const TranslationStr& fallback) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto& translation = TranslationCtx(domainSid, sourceSid, contextSid);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translation(const DomainStr& domain, const SourceStr& source, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::Translate(domain, source, _localeGenerator(localeString));
        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translation(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::Translate(domain, sourceSingular, sourcePlural, count, _localeGenerator(localeString));
        return Localization::Format(translation, count);
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::TranslateCtx(domain, source, context, _localeGenerator(localeString));
        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantVerbose);

        const auto translation = Localization::TranslateCtx(domain, sourceSingular, sourcePlural, count, context, _localeGenerator(localeString));
        return Localization::Format(translation, count);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::_ImbueLocale() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinor);

        std::locale::global(_localeGenerator(_currentLocale));
        const auto locale = std::locale();
        std::cout.imbue(locale);
        std::cerr.imbue(locale);
        std::clog.imbue(locale);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::_NotifyLocaleListeners() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        for (const auto& callback : _localeChangedCallbacks)
        {
            callback();
        }
    }
    //--------------------------------------------------------------------------
}