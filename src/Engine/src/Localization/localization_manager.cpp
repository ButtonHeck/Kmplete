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

    LocalizationManager::LocalizationManager()
        : _localeGenerator()
        , _library(CreateUPtr<LocalizationLibrary>())
        , _currentLocale(std::locale().name())
    {
        const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
        KMP_ASSERT(defaultTranslationsPath != LocalesDirectory);
        AddMessagesPath(defaultTranslationsPath);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::SetLocale(const LocaleStr& localeString)
    {
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

    const LocaleStr& LocalizationManager::GetLocale() const KMP_NOEXCEPT
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

    TranslationStr LocalizationManager::Translate(const DomainStr& domain, const SourceStr& message)
    {
        const auto translation = Translator::Translate(domain, message);
        _library->Add(Utils::ToStringID(domain), Utils::ToStringID(message), translation);
        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::Translate(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count) const
    {
        return Translator::Translate(domain, messageSingular, messagePlural, count);
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslateCtx(const DomainStr& domain, const SourceStr& message, const ContextStr& context)
    {
        const auto translation = Translator::TranslateCtx(domain, message, context);
        _library->Add(Utils::ToStringID(domain), Utils::ToStringID(message), Utils::ToStringID(context), translation);
        return translation;
    }
    //--------------------------------------------------------------------------

    TranslationStr LocalizationManager::TranslateCtx(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count, const ContextStr& context) const
    {
        return Translator::TranslateCtx(domain, messageSingular, messagePlural, count, context);
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStr& domain, const SourceStr& message) const
    {
        return _library->Get(Utils::ToStringID(domain), Utils::ToStringID(message));
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::Translation(const DomainStr& domain, const SourceStr& message, const ContextStr& context)
    {
        return _library->Get(Utils::ToStringID(domain), Utils::ToStringID(message), Utils::ToStringID(context));
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrFallback(const DomainStr& domain, const SourceStr& message, const TranslationStr& fallback) const
    {
        const auto& translation = Translation(domain, message);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrFallback(const DomainStr& domain, const SourceStr& message, const ContextStr& context, const TranslationStr& fallback)
    {
        const auto& translation = Translation(domain, message, context);
        return translation.empty() ? fallback : translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrSource(const DomainStr& domain, const SourceStr& message) const
    {
        const auto& translation = Translation(domain, message);
        return translation.empty() ? message : translation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationManager::TranslationOrSource(const DomainStr& domain, const SourceStr& message, const ContextStr& context)
    {
        const auto& translation = Translation(domain, message, context);
        return translation.empty() ? message : translation;
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