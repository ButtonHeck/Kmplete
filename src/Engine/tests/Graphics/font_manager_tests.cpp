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

    const auto fontsCount = fontManager->FontsAssetsCount();
    REQUIRE(fontsCount == 1UL); // Default font only
}
//--------------------------------------------------------------------------

TEST_CASE("FontManager default font usage", "[graphics][font_manager][font]")
{
    UPtr<FontManager> fontManager;
    REQUIRE_NOTHROW(fontManager.reset(new FontManager()));
    REQUIRE(fontManager);

    bool ok = false;
    REQUIRE_NOTHROW(ok = fontManager->RemoveFontAsset(FontManager::DefaultFontSID));
    REQUIRE_FALSE(ok);

    Vector<Utils::StringID> sids;
    sids.push_back(FontManager::DefaultFontSID);
    REQUIRE_NOTHROW(fontManager->RemoveFontsAssets(sids));
    const auto fontsCount = fontManager->FontsAssetsCount();
    REQUIRE(fontsCount == 1UL); // Default font not deleted

    const Assets::FontAsset* defaultFontAsset = nullptr;
    REQUIRE_NOTHROW(defaultFontAsset = &(fontManager->GetFontAsset(FontManager::DefaultFontSID)));
    REQUIRE(defaultFontAsset->GetStringID() == FontManager::DefaultFontSID);

    const Utils::StringID garbageSid = 1234;
    REQUIRE_NOTHROW(defaultFontAsset = &(fontManager->GetFontAsset(garbageSid)));
    REQUIRE(defaultFontAsset->GetStringID() == FontManager::DefaultFontSID); // still default font

    BinaryBuffer garbageBuffer;
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(FontManager::DefaultFontSID, std::move(garbageBuffer)));
    REQUIRE_FALSE(ok);

    Filepath garbagePath;
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(FontManager::DefaultFontSID, garbagePath));
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
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(fontSid, fontPath));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 2UL);

    // checking font properties
    Assets::FontAsset* fontAsset = nullptr;
    REQUIRE_NOTHROW(fontAsset = &(fontManager->GetFontAsset(fontSid)));
    REQUIRE(fontAsset);
    REQUIRE(fontAsset->GetStringID() == fontSid);
    
    BinaryBuffer fontBuffer = fontAsset->GetFont().GetBuffer();
    REQUIRE(!fontBuffer.empty());

    {
        auto& fontParams = fontAsset->GetFont().GetParameters();
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
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(fontSid, fontPath));
    REQUIRE_FALSE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 2UL);

    // try adding same font from binary buffer
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(fontSid, std::move(fontBuffer)));
    REQUIRE_FALSE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 2UL);

    fontBuffer = fontAsset->GetFont().GetBuffer();

    // try remove font (single sid)
    REQUIRE_NOTHROW(ok = fontManager->RemoveFontAsset(fontSid));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 1UL);

    // adding font again
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(fontSid, fontPath));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 2UL);

    // try remove font (non-existing sid)
    REQUIRE_NOTHROW(ok = fontManager->RemoveFontAsset(1234));
    REQUIRE_FALSE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 2UL);

    // try remove font (vector of sids)
    Vector<Utils::StringID> sids;
    sids.push_back(1234);
    REQUIRE_NOTHROW(fontManager->RemoveFontsAssets(sids));
    REQUIRE(fontManager->FontsAssetsCount() == 2UL); // not deleted

    sids.push_back(fontSid);
    REQUIRE_NOTHROW(fontManager->RemoveFontsAssets(sids));
    REQUIRE(fontManager->FontsAssetsCount() == 1UL); // deleted

    // adding font from binary buffer
    REQUIRE_NOTHROW(ok = fontManager->CreateFontAsset(fontSid, std::move(fontBuffer)));
    REQUIRE(ok);
    REQUIRE(fontManager->FontsAssetsCount() == 2UL);

    // checking font properties
    fontAsset = nullptr;
    REQUIRE_NOTHROW(fontAsset = &(fontManager->GetFontAsset(fontSid)));
    REQUIRE(fontAsset);
    REQUIRE(fontAsset->GetStringID() == fontSid);

    fontBuffer = fontAsset->GetFont().GetBuffer();
    REQUIRE(!fontBuffer.empty());

    {
        auto& fontParams = fontAsset->GetFont().GetParameters();
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
        REQUIRE_NOTHROW(ok = fontAsset->GetFont().SetPixelSize(20));
        REQUIRE(ok);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM == 20);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM == 20);

        REQUIRE_NOTHROW(ok = fontAsset->GetFont().SetPixelSize(0));
        REQUIRE(ok);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM != 20);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM != 20);

        // set font point size (default dpi)
        REQUIRE_NOTHROW(ok = fontAsset->GetFont().SetPointSize(20));
        REQUIRE(ok);
        const auto defaultDpiXPixelsPerEM = fontParams.sizeMetrics.xPixelsPerEM;
        const auto defaultDpiYPixelsPerEM = fontParams.sizeMetrics.yPixelsPerEM;
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM > 0);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM > 0);

        // set font point size (custom dpi)
        REQUIRE_NOTHROW(ok = fontAsset->GetFont().SetPointSize(20, 108));
        REQUIRE(ok);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM > 0);
        REQUIRE(fontParams.sizeMetrics.xPixelsPerEM != defaultDpiXPixelsPerEM);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM > 0);
        REQUIRE(fontParams.sizeMetrics.yPixelsPerEM != defaultDpiYPixelsPerEM);
    }
}
//--------------------------------------------------------------------------