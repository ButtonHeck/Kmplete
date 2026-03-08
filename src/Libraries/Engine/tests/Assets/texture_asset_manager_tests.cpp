#include "Kmplete/Assets/texture_asset_manager.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Utils/string_utils.h"

#include <catch2/catch_test_macros.hpp>
#include <GLFW/glfw3.h>


using namespace Kmplete;
using namespace Kmplete::Assets;
using namespace Kmplete::Graphics;


static UPtr<GraphicsBackend> prepareBackend(GraphicsBackendType type)
{
    auto windowBackend = Kmplete::WindowBackend::Create();
    windowBackend->SetGraphicsBackendType(type);
    auto& mainWindow = windowBackend->CreateMainWindow();

    return GraphicsBackend::Create(mainWindow);
}
//--------------------------------------------------------------------------


TEST_CASE("TextureAssetManager initialization Vulkan", "[graphics][texture_asset_manager][asset]")
{
    const auto graphicsBackend = prepareBackend(GraphicsBackendType::Vulkan);

    UPtr<TextureAssetManager> textureAssetManager;
    REQUIRE_NOTHROW(textureAssetManager = CreateUPtr<TextureAssetManager>(*graphicsBackend.get()));
    REQUIRE(textureAssetManager);
}
//--------------------------------------------------------------------------


TEST_CASE("TextureAssetManager default texture usage", "[graphics][texture_asset_manager][texture][asset]")
{
    const auto graphicsBackend = prepareBackend(GraphicsBackendType::Vulkan);

    UPtr<TextureAssetManager> textureAssetManager;
    REQUIRE_NOTHROW(textureAssetManager = CreateUPtr<TextureAssetManager>(*graphicsBackend.get()));
    REQUIRE(textureAssetManager);

    bool ok = false;
    REQUIRE_NOTHROW(ok = textureAssetManager->RemoveAsset(TextureAssetManager::ErrorTextureSID));
    REQUIRE_FALSE(ok);

    Vector<StringID> sids;
    sids.push_back(TextureAssetManager::ErrorTextureSID);
    REQUIRE_NOTHROW(textureAssetManager->RemoveAssets(sids));
    const auto texturesCount = textureAssetManager->GetAssetsCount();
    REQUIRE(texturesCount == 1UL); // error texture not deleted

    const Assets::TextureAsset* errorTextureAsset = nullptr;
    REQUIRE_NOTHROW(errorTextureAsset = &(textureAssetManager->GetAsset(TextureAssetManager::ErrorTextureSID)));
    REQUIRE(errorTextureAsset->GetStringID() == TextureAssetManager::ErrorTextureSID);

    const StringID garbageSid = 1234;
    REQUIRE_NOTHROW(errorTextureAsset = &(textureAssetManager->GetAsset(garbageSid)));
    REQUIRE(errorTextureAsset->GetStringID() == TextureAssetManager::ErrorTextureSID); // still error texture

    Filepath garbagePath;
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(TextureAssetManager::ErrorTextureSID, garbagePath));
    REQUIRE_FALSE(ok);
}
//--------------------------------------------------------------------------


TEST_CASE("TextureAssetManager texture functions", "[graphics][texture_asset_manager][texture][asset]")
{
    const auto graphicsBackend = prepareBackend(GraphicsBackendType::Vulkan);

    UPtr<TextureAssetManager> textureAssetManager;
    REQUIRE_NOTHROW(textureAssetManager = CreateUPtr<TextureAssetManager>(*graphicsBackend.get()));
    REQUIRE(textureAssetManager);

    // loading from image
    const auto image = Image(Filepath(KMP_TEST_ICON_PATH), ImageChannels::RGBAlpha);
    const StringID imageSid = 12345UL;
    bool ok = false;
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(imageSid, image));
    REQUIRE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL);

    // checking texture properties
    Assets::TextureAsset* textureAsset = nullptr;
    REQUIRE_NOTHROW(textureAsset = &(textureAssetManager->GetAsset(imageSid)));
    REQUIRE(textureAsset);
    REQUIRE(textureAsset->GetStringID() == imageSid);

    // try adding same texture from image
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(imageSid, image));
    REQUIRE_FALSE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL);

    // try adding same texture from filepath
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(imageSid, Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE_FALSE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL);

    // try remove texture (single sid)
    REQUIRE_NOTHROW(ok = textureAssetManager->RemoveAsset(imageSid));
    REQUIRE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 1UL);

    // add texture again
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(imageSid, image));
    REQUIRE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL);

    // try remove texture (invalid sid)
    REQUIRE_NOTHROW(ok = textureAssetManager->RemoveAsset(8888));
    REQUIRE_FALSE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL);

    // try remove texture (vector of sids)
    Vector<StringID> sids;
    sids.push_back(8888);
    REQUIRE_NOTHROW(textureAssetManager->RemoveAssets(sids));
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL); // not deleted

    sids.push_back(imageSid);
    REQUIRE_NOTHROW(textureAssetManager->RemoveAssets(sids));
    REQUIRE(textureAssetManager->GetAssetsCount() == 1UL); // deleted

    // adding texture from filepath
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(imageSid, Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 2UL);

    // adding texture from non-image filepath (removing first)
    REQUIRE_NOTHROW(ok = textureAssetManager->RemoveAsset(imageSid));
    REQUIRE(ok);
    REQUIRE_NOTHROW(ok = textureAssetManager->CreateAsset(imageSid, Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf")));
    REQUIRE_FALSE(ok);
    REQUIRE(textureAssetManager->GetAssetsCount() == 1UL);
}
//--------------------------------------------------------------------------