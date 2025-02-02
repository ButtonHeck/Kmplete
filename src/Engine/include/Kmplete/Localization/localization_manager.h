#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Localization/localization_base.h"

#include <boost/locale.hpp>

#include <vector>
#include <functional>

namespace Kmplete
{
    class Settings;

    class LocalizationManager
    {
    public:
        using LocaleChangeCallback = std::function<void()>;

    public:
        KMP_DISABLE_COPY_MOVE(LocalizationManager)

        KMP_API LocalizationManager();
        ~LocalizationManager() = default;

        KMP_API bool SetLocale(const LocaleStr& localeString);
        KMP_NODISCARD KMP_API const LocaleStr& GetLocale() const KMP_NOEXCEPT;

        KMP_API void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

        void SaveSettings(Settings& settings);
        void LoadSettings(Settings& settings);

    private:
        void ImbueLocale(const std::locale& locale) const;
        void NotifyLocaleListeners() const;

    private:
        boost::locale::generator _localeGenerator;
        LocaleStr _currentLocale;
        std::vector<LocaleChangeCallback> _localeChangedCallbacks;
    };
    //--------------------------------------------------------------------------
}