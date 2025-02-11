#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/nullability.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Localization/localization_dictionary.h"

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
        KMP_NODISCARD KMP_API const LocaleStr& GetLocale() const noexcept;

        KMP_API void AddMessagesPath(const std::string& path);

        KMP_API NonNull<Ptr<LocalizationDictionary>> AddMessagesDomain(const DomainStr& domain);
        KMP_API void RemoveMessagesDomain(const DomainStr& domain);

        KMP_NODISCARD KMP_API Nullable<Ptr<LocalizationDictionary>> GetDictionary(const DomainStr& domain) const;

        KMP_API void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

        void SaveSettings(Settings& settings) const;
        void LoadSettings(Settings& settings);

        KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& message);
        KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count) const;
        KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& message, const ContextStr& context);
        KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count, const ContextStr& context) const;

        constexpr static void TranslateDefer(const DomainStr&, const SourceStr&) {};
        constexpr static void TranslateDefer(const DomainStr&, const SourceStr&, const SourceStr&, int) {};
        constexpr static void TranslateCtxDefer(const DomainStr&, const SourceStr&, const ContextStr&) {};
        constexpr static void TranslateCtxDefer(const DomainStr&, const SourceStr&, const SourceStr&, int, const ContextStr&) {};

        KMP_NODISCARD KMP_API const TranslationStr& Translation(const DomainStr& domain, const SourceStr& message) const;
        KMP_NODISCARD KMP_API const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& messageSid) const;
        KMP_NODISCARD KMP_API const TranslationStr& Translation(const DomainStr& domain, const SourceStr& message, const ContextStr& context);
        KMP_NODISCARD KMP_API const TranslationStr& TranslationOrFallback(const DomainStr& domain, const SourceStr& message, const TranslationStr& fallback) const;
        KMP_NODISCARD KMP_API const TranslationStr& TranslationOrFallback(const DomainStr& domain, const SourceStr& message, const ContextStr& context, const TranslationStr& fallback);
        KMP_NODISCARD KMP_API const TranslationStr& TranslationOrSource(const DomainStr& domain, const SourceStr& message) const;
        KMP_NODISCARD KMP_API const TranslationStr& TranslationOrSource(const DomainStr& domain, const SourceStr& message, const ContextStr& context);

    private:
        void ImbueLocale() const;
        void NotifyLocaleListeners() const;

    private:
        boost::locale::generator _localeGenerator;
        UPtr<LocalizationLibrary> _library;
        LocaleStr _currentLocale;
        std::vector<LocaleChangeCallback> _localeChangedCallbacks;
    };
    //--------------------------------------------------------------------------
}