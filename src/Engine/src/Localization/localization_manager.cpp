#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_translator.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/assertion.h"

#include <iostream>

namespace Kmplete
{
    constexpr static auto LocalizationManagerSettingsEntryName = "LocalizationManager";
    constexpr static auto LocalizationManagerLocaleStr = "Locale";

    LocalizationManager::LocalizationManager() noexcept
        : _localeGenerator()
        , _library(CreateUPtr<LocalizationLibrary>())
        , _currentLocale(std::locale().name())
    {}
    //--------------------------------------------------------------------------

    bool LocalizationManager::SetLocale(const LocaleStr& localeString)
    {
        if (_currentLocale == localeString)
        {
            KMP_LOG_CORE_INFO("LocalizationManager: locale '{}' already set", localeString);
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

            KMP_LOG_CORE_INFO("LocalizationManager: set locale '{}'", _currentLocale);
            return true;
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_CORE_ERROR("LocalizationManager: cannot set locale '{}' - {}", localeString, e.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    const LocaleStr& LocalizationManager::GetLocale() const noexcept
    {
        return _currentLocale;
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const std::string& path)
    {
        _localeGenerator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    NonNull<Ptr<LocalizationDictionary>> LocalizationManager::AddMessagesDomain(const DomainStr& domain)
    {
        _localeGenerator.add_messages_domain(domain);
        ImbueLocale();

        return _library->AddDictionary(domain);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::RemoveMessagesDomain(const DomainStr& domain)
    {
        _library->RemoveDictionary(domain);
    }
    //--------------------------------------------------------------------------

    Nullable<Ptr<LocalizationDictionary>> LocalizationManager::GetDictionary(const DomainStr& domain) const
    {
        return _library->GetDictionary(domain);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
    {
        _localeChangedCallbacks.push_back(callback);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(LocalizationManagerSettingsEntryName);
        settings.SaveString(LocalizationManagerLocaleStr, _currentLocale);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(LocalizationManagerSettingsEntryName);
        const auto localeStr = settings.GetString(LocalizationManagerLocaleStr, LocaleEnUTF8Keyword);
        SetLocale(localeStr);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStrSID& domainSid, const SourceStrSID& messageSid) const
    {
        return _library->Get(domainSid, messageSid);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStrSID& domainSid, const SourceStrSID& messageSid, const ContextStrSID& contextSid) const
    {
        return _library->Get(domainSid, messageSid, contextSid);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& messageSid, const TranslationStr& fallback) const
    {
        const auto& translation = Translation(domainSid, messageSid);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& messageSid, const ContextStrSID& contextSid, const TranslationStr& fallback) const
    {
        const auto& translation = Translation(domainSid, messageSid, contextSid);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::ImbueLocale() const
    {
        std::locale::global(_localeGenerator(_currentLocale));
        std::cout.imbue(std::locale());
        std::cerr.imbue(std::locale());
        std::clog.imbue(std::locale());
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::NotifyLocaleListeners() const
    {
        for (const auto& callback : _localeChangedCallbacks)
        {
            callback();
        }
    }
    //--------------------------------------------------------------------------
}