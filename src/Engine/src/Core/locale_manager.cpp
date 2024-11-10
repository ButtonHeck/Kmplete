#include "Kmplete/Core/locale_manager.h"
#include "Kmplete/Core/log.h"

#include <iostream>

namespace Kmplete
{
    LocaleManager::LocaleManager()
        : _localeGenerator()
        , _currentLocale(std::locale().name())
    {}
    //--------------------------------------------------------------------------

    bool LocaleManager::SetLocale(const std::string& localeString)
    {
        try
        {
            const auto testLocale = localeString.empty() ? std::locale::classic() : std::locale(localeString);
            _currentLocale = testLocale.name();
            const auto newLocale = std::locale::global(_localeGenerator.generate(localeString));
            std::setlocale(LC_ALL, _currentLocale.c_str());

            ImbueLocale(newLocale);
            NotifyLocaleListeners();

            KMP_LOG_CORE_INFO("LocaleManager: set locale '{}'", _currentLocale);
            return true;
        }
        catch (KMP_MB_UNUSED const std::exception& e)
        {
            KMP_LOG_CORE_ERROR("LocaleManager: cannot set locale '{}' - {}", localeString, e.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    void LocaleManager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
    {
        _localeChangedCallbacks.push_back(callback);
    }
    //--------------------------------------------------------------------------

    const std::string& LocaleManager::GetLocale() const KMP_NOEXCEPT
    {
        return _currentLocale;
    }
    //--------------------------------------------------------------------------

    void LocaleManager::ImbueLocale(const std::locale& locale) const
    {
        std::cout.imbue(locale);
        std::cerr.imbue(locale);
        std::clog.imbue(locale);
    }
    //--------------------------------------------------------------------------

    void Kmplete::LocaleManager::NotifyLocaleListeners() const
    {
        for (const auto& callback : _localeChangedCallbacks)
        {
            callback();
        }
    }
    //--------------------------------------------------------------------------
}