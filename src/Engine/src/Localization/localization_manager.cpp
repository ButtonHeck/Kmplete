#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/settings.h"

#include <iostream>

namespace Kmplete
{
    constexpr static auto LocalizationManagerSettingsEntryName = "LocalizationManager";
    constexpr static auto LocalizationManagerLocaleStr = "Locale";

    LocalizationManager::LocalizationManager()
        : _localeGenerator()
        , _currentLocale(std::locale().name())
    {}
    //--------------------------------------------------------------------------

    bool LocalizationManager::SetLocale(const LocaleStr& localeString)
    {
        try
        {
            const auto testLocale = localeString.empty() ? std::locale::classic() : std::locale(localeString);
            _currentLocale = testLocale.name();
            const auto newLocale = std::locale::global(_localeGenerator.generate(localeString));
            std::setlocale(LC_ALL, _currentLocale.c_str());

            ImbueLocale(newLocale);
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

    void LocalizationManager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
    {
        _localeChangedCallbacks.push_back(callback);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::SaveSettings(Settings& settings)
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

    const LocaleStr& LocalizationManager::GetLocale() const KMP_NOEXCEPT
    {
        return _currentLocale;
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::ImbueLocale(const std::locale& locale) const
    {
        std::cout.imbue(locale);
        std::cerr.imbue(locale);
        std::clog.imbue(locale);
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