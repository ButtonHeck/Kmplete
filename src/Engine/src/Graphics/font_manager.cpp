#include "Kmplete/Graphics/font_manager.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>


namespace Kmplete
{
    FontManager::FontManager()
        : _freetypeLibInstance(nullptr)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto freetypeInitError = FT_Init_FreeType(&_freetypeLibInstance);
        if (freetypeInitError)
        {
            KMP_LOG_CRITICAL("failed to initialize FreeType library instance");
            throw std::runtime_error("FontManager: failed to initialize FreeType library instance");
        }
    }
    //--------------------------------------------------------------------------

    FontManager::~FontManager()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _fonts.clear();

        const auto freetypeDoneError = FT_Done_FreeType(_freetypeLibInstance);
        if (freetypeDoneError)
        {
            KMP_LOG_ERROR("failed to shutdown FreeType library instance");
        }
    }
    //--------------------------------------------------------------------------

    bool FontManager::CreateFontTTF(Utils::StringID fontSid, const BinaryBuffer& fontData)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_fonts.contains(fontSid))
        {
            KMP_LOG_ERROR("already contains font with sid '{}'", fontSid);
            return false;
        }

        const auto result = _fonts.emplace(fontSid, CreateUPtr<Font>(fontSid, *_freetypeLibInstance, fontData));
        return result.second;
    }
    //--------------------------------------------------------------------------

    OptionalRef<const Font> FontManager::GetFont(Utils::StringID fontSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        if (!_fonts.contains(fontSid))
        {
            KMP_LOG_ERROR("cannot find a font with sid '{}'", fontSid);
            return std::nullopt;
        }

        return std::cref(*_fonts.at(fontSid).get());
    }
    //--------------------------------------------------------------------------
}