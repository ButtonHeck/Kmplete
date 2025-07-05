#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_translator.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"

#include <iostream>

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "LocalizationManager";
    constexpr static auto SettingsLocaleStr = "Locale";

    LocalizationManager::LocalizationManager() noexcept
        : _localeGenerator()
        , _library(CreateUPtr<LocalizationLibrary>())
        , _currentLocale(std::locale().name())
    {
        KMP_PROFILE_FUNCTION();

        SetLocale(LocaleEnUTF8Keyword);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::SetLocale(const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION();

        if (_currentLocale == localeString)
        {
            KMP_LOG_INFO("LocalizationManager: locale '{}' already set", localeString);
            return false;
        }

        try
        {
            const auto testLocale = localeString.empty() ? std::locale::classic() : std::locale(localeString);
            _currentLocale = testLocale.name();
            std::setlocale(LC_ALL, _currentLocale.c_str());

            _library->SetLocale(_currentLocale);

            ImbueLocale();
            NotifyLocaleListeners();

            KMP_LOG_INFO("LocalizationManager: set locale '{}'", _currentLocale);
            return true;
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_ERROR("LocalizationManager: cannot set locale '{}' - {}", localeString, e.what());
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
        KMP_PROFILE_FUNCTION();

        _localeGenerator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::AddMessagesDomain(const DomainStr& domain)
    {
        KMP_PROFILE_FUNCTION();

        _localeGenerator.add_messages_domain(domain);
        ImbueLocale();

        return _library->AddDictionary(domain);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::RemoveMessagesDomain(const DomainStr& domain)
    {
        KMP_PROFILE_FUNCTION();

        return _library->RemoveDictionary(domain);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
    {
        KMP_PROFILE_FUNCTION();

        _localeChangedCallbacks.push_back(callback);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::SaveSettings(Settings& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveString(SettingsLocaleStr, _currentLocale);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::LoadSettings(Settings& settings)
    {
        KMP_PROFILE_FUNCTION();

        settings.StartLoadObject(SettingsEntryName);
        const auto localeStr = settings.GetString(SettingsLocaleStr, LocaleEnUTF8Keyword);
        SetLocale(localeStr);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translate(const DomainStr& domain, const SourceStr& source)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::Translate(domain, source);
        const auto domainSid = Utils::ToStringID(domain);
        const auto sourceSid = Utils::ToStringID(source);
        _library->Add(domainSid, sourceSid, translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::Translate(domain, sourceSingular, sourcePlural, count);
        const auto domainSid = Utils::ToStringID(domain);
        const auto sourceSingularSid = Utils::ToStringID(sourceSingular);
        const auto sourcePluralSid = Utils::ToStringID(sourcePlural);
        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        _library->Add(domainSid, sourceSingularSid, sourcePluralSid, pluralityDispatcher.GetPluralityForm(count), translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::TranslateCtx(domain, source, context);
        const auto domainSid = Utils::ToStringID(domain);
        const auto sourceSid = Utils::ToStringID(source);
        const auto contextSid = Utils::ToStringID(context);
        _library->Add(domainSid, sourceSid, contextSid, translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, 
                                                     int count, const ContextStr& context)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::TranslateCtx(domain, sourceSingular, sourcePlural, count, context);
        const auto domainSid = Utils::ToStringID(domain);
        const auto sourceSingularSid = Utils::ToStringID(sourceSingular);
        const auto sourcePluralSid = Utils::ToStringID(sourcePlural);
        const auto contextSid = Utils::ToStringID(context);
        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        _library->Add(domainSid, sourceSingularSid, sourcePluralSid, pluralityDispatcher.GetPluralityForm(count), contextSid, translation);

        return translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const
    {
        KMP_PROFILE_FUNCTION();

        return _library->Get(domainSid, sourceSid);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                           const SourceStrSID& sourceSidPlural, int count) const
    {
        KMP_PROFILE_FUNCTION();

        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        return _library->Get(domainSid, sourceSidSingular, sourceSidPlural, pluralityDispatcher.GetPluralityForm(count));
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                             const SourceStrSID& sourceSidPlural, int count) const
    {
        KMP_PROFILE_FUNCTION();

        return Localization::Format(Translation(domainSid, sourceSidSingular, sourceSidPlural, count), count);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                              const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION();

        const auto& pluralityDispatcher = GetPluralityDispatcher(_currentLocale);
        return _library->Get(domainSid, sourceSidSingular, sourceSidPlural, pluralityDispatcher.GetPluralityForm(count), contextSid);
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationCtxFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                                const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION();

        return Localization::Format(TranslationCtx(domainSid, sourceSidSingular, sourceSidPlural, count, contextSid), count);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION();

        return _library->Get(domainSid, sourceSid, contextSid);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& fallback) const
    {
        KMP_PROFILE_FUNCTION();

        const auto& translation = Translation(domainSid, sourceSid);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationCtxOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, 
                                                                        const ContextStrSID& contextSid, const TranslationStr& fallback) const
    {
        KMP_PROFILE_FUNCTION();

        const auto& translation = TranslationCtx(domainSid, sourceSid, contextSid);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translation(const DomainStr& domain, const SourceStr& source, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::Translate(domain, source, _localeGenerator(localeString));
        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translation(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::Translate(domain, sourceSingular, sourcePlural, count, _localeGenerator(localeString));
        return Localization::Format(translation, count);
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::TranslateCtx(domain, source, context, _localeGenerator(localeString));
        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslationCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const LocaleStr& localeString)
    {
        KMP_PROFILE_FUNCTION();

        const auto translation = Localization::TranslateCtx(domain, sourceSingular, sourcePlural, count, context, _localeGenerator(localeString));
        return Localization::Format(translation, count);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::ImbueLocale() const
    {
        KMP_PROFILE_FUNCTION();

        std::locale::global(_localeGenerator(_currentLocale));
        const auto locale = std::locale();
        std::cout.imbue(locale);
        std::cerr.imbue(locale);
        std::clog.imbue(locale);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::NotifyLocaleListeners() const
    {
        KMP_PROFILE_FUNCTION();

        for (const auto& callback : _localeChangedCallbacks)
        {
            callback();
        }
    }
    //--------------------------------------------------------------------------
}