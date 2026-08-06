#include "Kmplete/Localization/localization_unicode_map.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Localization
    {
        /*static*/ bool UnicodeMap::_initialized = false;
        /*static*/ HashMap<LocaleStr, CodepointVector> UnicodeMap::_localeCodepointRanges;


        bool UnicodeMap::Initialize()
        {
            if (_initialized)
            {
                KMP_LOG_WARN("already initialized");
                return true;
            }

            if (not _InitializeEn())
            {
                KMP_LOG_ERROR("failed to initialize codepoints vector for locale '{}'", LocaleEnUTF8Keyword);
                return false;
            }
            if (not _InitializeRu())
            {
                KMP_LOG_ERROR("failed to initialize codepoints vector for locale '{}'", LocaleRuUTF8Keyword);
                return false;
            }

            KMP_LOG_INFO("initialization completed");
            _initialized = true;

            return true;
        }
        //--------------------------------------------------------------------------

        const CodepointVector& UnicodeMap::GetCodepointVector(const LocaleStr& locale)
        {
            static const CodepointVector invalidVector{};
            
            if (not _localeCodepointRanges.contains(locale))
            {
                KMP_LOG_ERROR("cannot find codepoints for locale '{}'", locale);
                return invalidVector;
            }

            return _localeCodepointRanges[locale];
        }
        //--------------------------------------------------------------------------

        bool UnicodeMap::_InitializeEn()
        {
            CodepointVector localeCodes;

            static constexpr wchar_t basicLatinCapitalLettersStart = 0x0041;
            static constexpr wchar_t basicLatinCapitalLettersEnd = 0x005A;
            for (wchar_t c = basicLatinCapitalLettersStart; c <= basicLatinCapitalLettersEnd; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t basicLatinSmallLettersStart = 0x0061;
            static constexpr wchar_t basicLatinSmallLettersEnd = 0x007A;
            for (wchar_t c = basicLatinSmallLettersStart; c <= basicLatinSmallLettersEnd; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t basicLatinSymbolsStart = 0x0020;
            static constexpr wchar_t basicLatinSymbolsEnd = 0x0040;
            for (wchar_t c = basicLatinSymbolsStart; c <= basicLatinSymbolsEnd; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t basicLatinSymbols2Start = 0x005B;
            static constexpr wchar_t basicLatinSymbols2End = 0x0060;
            for (wchar_t c = basicLatinSymbols2Start; c <= basicLatinSymbols2End; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t basicLatinSymbols3Start = 0x007B;
            static constexpr wchar_t basicLatinSymbols3End = 0x007E;
            for (wchar_t c = basicLatinSymbols3Start; c <= basicLatinSymbols3End; c++)
            {
                localeCodes.push_back(c);
            }

            const auto [iterator, hasEmplaced] = _localeCodepointRanges.emplace(LocaleEnUTF8Keyword, localeCodes);
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------

        bool UnicodeMap::_InitializeRu()
        {
            CodepointVector localeCodes;

            static constexpr wchar_t cyrillicStart = 0x0410;
            static constexpr wchar_t cyrillicEnd = 0x044F;
            for (wchar_t c = cyrillicStart; c <= cyrillicEnd; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t cyrillicCapitalIo = 0x0401;
            static constexpr wchar_t cyrillicSmallIo = 0x0451;
            localeCodes.push_back(cyrillicCapitalIo);
            localeCodes.push_back(cyrillicSmallIo);

            static constexpr wchar_t basicLatinSymbolsStart = 0x0020;
            static constexpr wchar_t basicLatinSymbolsEnd = 0x0040;
            for (wchar_t c = basicLatinSymbolsStart; c <= basicLatinSymbolsEnd; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t basicLatinSymbols2Start = 0x005B;
            static constexpr wchar_t basicLatinSymbols2End = 0x0060;
            for (wchar_t c = basicLatinSymbols2Start; c <= basicLatinSymbols2End; c++)
            {
                localeCodes.push_back(c);
            }

            static constexpr wchar_t basicLatinSymbols3Start = 0x007B;
            static constexpr wchar_t basicLatinSymbols3End = 0x007E;
            for (wchar_t c = basicLatinSymbols3Start; c <= basicLatinSymbols3End; c++)
            {
                localeCodes.push_back(c);
            }

            const auto [iterator, hasEmplaced] = _localeCodepointRanges.emplace(LocaleRuUTF8Keyword, localeCodes);
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------
    }
}