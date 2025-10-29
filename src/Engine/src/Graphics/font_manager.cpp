#include "Kmplete/Graphics/font_manager.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace Kmplete
{
    FontManager::FontManager()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto freeTypeInitError = FT_Init_FreeType(&_freetypeLibInstance);
        if (freeTypeInitError)
        {
            KMP_LOG_CRITICAL("Failed to initialize FreeType library instance");
            throw std::runtime_error("FontManager: failed to initialize FreeType library instance");
        }
    }
    //--------------------------------------------------------------------------

    FontManager::~FontManager()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto freeTypeDoneError = FT_Done_FreeType(_freetypeLibInstance);
        if (freeTypeDoneError)
        {
            KMP_LOG_ERROR("Failed to shutdown FreeType library instance");
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

        const auto result = _fonts.emplace(fontSid, fontData);
        return result.second;
    }
    //--------------------------------------------------------------------------

    OptionalRef<const BinaryBuffer> FontManager::GetFont(Utils::StringID fontSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        if (!_fonts.contains(fontSid))
        {
            KMP_LOG_ERROR("cannot find a font with sid '{}'", fontSid);
            return std::nullopt;
        }

        return std::cref(_fonts.at(fontSid));
    }
    //--------------------------------------------------------------------------
}