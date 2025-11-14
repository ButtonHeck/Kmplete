#include "Kmplete/Graphics/font_manager.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Utils/string_utils.h"

#include <catch2/catch_test_macros.hpp>


using namespace Kmplete;

TEST_CASE("FontManager creation", "[graphics][font_manager][font]")
{
    UPtr<FontManager> fontManager;
    REQUIRE_NOTHROW(fontManager.reset(new FontManager()));
    REQUIRE(fontManager);

    const auto fontsCount = fontManager->FontsCount();
    REQUIRE(fontsCount == 1UL); // Default font only
}
//--------------------------------------------------------------------------

TEST_CASE("FontManager default font usage", "[graphics][font_manager][font]")
{
    UPtr<FontManager> fontManager;
    REQUIRE_NOTHROW(fontManager.reset(new FontManager()));
    REQUIRE(fontManager);

    bool ok = false;
    REQUIRE_NOTHROW(ok = fontManager->RemoveFont(FontManager::DefaultFontSID));
    REQUIRE_FALSE(ok);

    Vector<Utils::StringID> sids;
    sids.push_back(FontManager::DefaultFontSID);
    REQUIRE_NOTHROW(fontManager->RemoveFonts(sids));
    const auto fontsCount = fontManager->FontsCount();
    REQUIRE(fontsCount == 1UL); // Default font not deleted

    const Assets::FontAsset* defaultFont = nullptr;
    REQUIRE_NOTHROW(defaultFont = &(fontManager->GetFont(FontManager::DefaultFontSID)));
    REQUIRE(defaultFont->GetStringID() == FontManager::DefaultFontSID);

    const Utils::StringID garbageSid = 1234;
    REQUIRE_NOTHROW(defaultFont = &(fontManager->GetFont(garbageSid)));
    REQUIRE(defaultFont->GetStringID() == FontManager::DefaultFontSID); // still default font

    BinaryBuffer garbageBuffer;
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(FontManager::DefaultFontSID, std::move(garbageBuffer)));
    REQUIRE_FALSE(ok);

    Filepath garbagePath;
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(FontManager::DefaultFontSID, garbagePath));
    REQUIRE_FALSE(ok);
}
//--------------------------------------------------------------------------

TEST_CASE("FontManager font functions", "[graphics][font_manager][font]")
{
    UPtr<FontManager> fontManager;
    REQUIRE_NOTHROW(fontManager.reset(new FontManager()));
    REQUIRE(fontManager);

    // loading from filepath
    const auto fontPath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
    const auto fontSid = "OpenSans-Regular.ttf"_sid;
    bool ok = false;
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(fontSid, fontPath));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsCount() == 2UL);

    // checking font properties
    Assets::FontAsset* font = nullptr;
    REQUIRE_NOTHROW(font = &(fontManager->GetFont(fontSid)));
    REQUIRE(font);
    REQUIRE(font->GetStringID() == fontSid);
    
    BinaryBuffer fontBuffer = font->GetFont().GetBuffer();
    REQUIRE(!fontBuffer.empty());

    {
        auto& fontParams = font->GetFont().GetParameters();
        REQUIRE(!fontParams.familyName.empty());
        REQUIRE(fontParams.numGlyphs > 0);
        REQUIRE(fontParams.unitsPerEM > 0);
        REQUIRE(fontParams.lineHeight != 0);
        REQUIRE(fontParams.ascender != 0);
        REQUIRE(fontParams.descender != 0);
        REQUIRE(fontParams.maxAdvance != 0);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM != 0);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM != 0);
        REQUIRE(fontParams.sizeMetrics.height != 0);
    }

    // try adding same font from filepath
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(fontSid, fontPath));
    REQUIRE_FALSE(ok);
    REQUIRE(fontManager->FontsCount() == 2UL);

    // try adding same font from binary buffer
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(fontSid, std::move(fontBuffer)));
    REQUIRE_FALSE(ok);
    REQUIRE(fontManager->FontsCount() == 2UL);

    fontBuffer = font->GetFont().GetBuffer();

    // try remove font (single sid)
    REQUIRE_NOTHROW(ok = fontManager->RemoveFont(fontSid));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsCount() == 1UL);

    // adding font again
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(fontSid, fontPath));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsCount() == 2UL);

    // try remove font (non-existing sid)
    REQUIRE_NOTHROW(ok = fontManager->RemoveFont(1234));
    REQUIRE_FALSE(ok);
    REQUIRE(fontManager->FontsCount() == 2UL);

    // try remove font (vector of sids)
    Vector<Utils::StringID> sids;
    sids.push_back(1234);
    REQUIRE_NOTHROW(fontManager->RemoveFonts(sids));
    REQUIRE(fontManager->FontsCount() == 2UL); // not deleted

    sids.push_back(fontSid);
    REQUIRE_NOTHROW(fontManager->RemoveFonts(sids));
    REQUIRE(fontManager->FontsCount() == 1UL); // deleted

    // adding font from binary buffer
    REQUIRE_NOTHROW(ok = fontManager->CreateFontTTF(fontSid, std::move(fontBuffer)));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsCount() == 2UL);

    // checking font properties
    font = nullptr;
    REQUIRE_NOTHROW(font = &(fontManager->GetFont(fontSid)));
    REQUIRE(font);
    REQUIRE(font->GetStringID() == fontSid);

    fontBuffer = font->GetFont().GetBuffer();
    REQUIRE(!fontBuffer.empty());

    {
        auto& fontParams = font->GetFont().GetParameters();
        REQUIRE(!fontParams.familyName.empty());
        REQUIRE(fontParams.numGlyphs > 0);
        REQUIRE(fontParams.unitsPerEM > 0);
        REQUIRE(fontParams.lineHeight != 0);
        REQUIRE(fontParams.ascender != 0);
        REQUIRE(fontParams.descender != 0);
        REQUIRE(fontParams.maxAdvance != 0);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM != 0);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM != 0);
        REQUIRE(fontParams.sizeMetrics.height != 0);

        // set font pixel size
        REQUIRE_NOTHROW(ok = font->GetFont().SetPixelSize(20));
        REQUIRE(ok);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM == 20);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM == 20);

        REQUIRE_NOTHROW(ok = font->GetFont().SetPixelSize(0));
        REQUIRE(ok);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM != 20);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM != 20);

        // set font point size (default dpi)
        REQUIRE_NOTHROW(ok = font->GetFont().SetPointSize(20));
        REQUIRE(ok);
        const auto defaultDpiXPixelsPerEM = fontParams.sizeMetrics.xPixelsPerEM;
        const auto defaultDpiYPixelsPerEM = fontParams.sizeMetrics.yPixelsPerEM;
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM > 0);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM > 0);

        // set font point size (custom dpi)
        REQUIRE_NOTHROW(ok = font->GetFont().SetPointSize(20, 108));
        REQUIRE(ok);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM > 0);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM != defaultDpiXPixelsPerEM);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM > 0);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM != defaultDpiYPixelsPerEM);
    }
}
//--------------------------------------------------------------------------