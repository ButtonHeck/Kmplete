#include "Kmplete/Graphics/font_manager.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
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