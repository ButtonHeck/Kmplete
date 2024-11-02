#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <boost/locale.hpp>

#include <string>
#include <vector>
#include <functional>

namespace Kmplete
{
    class LocaleManager
    {
    public:
        typedef std::function<void()> LocaleChangeCallback;

    public:
        KMP_DISABLE_COPY_MOVE(LocaleManager)

        KMP_API LocaleManager();
        KMP_API ~LocaleManager() = default;

        KMP_API bool SetLocale(const std::string& localeString);
        KMP_NODISCARD KMP_API const std::string& GetLocale() const KMP_NOEXCEPT;

        KMP_API void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

    private:
        void ImbueLocale(const std::locale& locale) const;
        void NotifyLocaleListeners() const;

    private:
        boost::locale::generator _localeGenerator;
        std::string _currentLocale;
        std::vector<LocaleChangeCallback> _localeChangedCallbacks;
    };
    //--------------------------------------------------------------------------
}