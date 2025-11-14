#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Utils/string_utils.h"

#include <catch2/catch_test_macros.hpp>
#include <GLFW/glfw3.h>


using namespace Kmplete;

static UPtr<GraphicsBackend> prepareOpenGLBackend()
{
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    const auto window = glfwCreateWindow(100, 100, "", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    return GraphicsBackend::Create(GraphicsBackendType::OpenGL);
}
//--------------------------------------------------------------------------


TEST_CASE("TextureManager initialization error type", "[graphics][texture_manager]")
{
    prepareOpenGLBackend();

    UPtr<TextureManager> textureManager;
    REQUIRE_THROWS(textureManager = CreateUPtr<TextureManager>(GraphicsBackendType::Unknown));
    REQUIRE(textureManager == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("TextureManager initialization opengl", "[graphics][texture_manager]")
{
    prepareOpenGLBackend();

    UPtr<TextureManager> textureManager;
    REQUIRE_NOTHROW(textureManager = CreateUPtr<TextureManager>(GraphicsBackendType::OpenGL));
    REQUIRE(textureManager);
}
//--------------------------------------------------------------------------

TEST_CASE("TextureManager default texture usage", "[graphics][texture_manager][texture]")
{
    prepareOpenGLBackend();

    UPtr<TextureManager> textureManager;
    REQUIRE_NOTHROW(textureManager = CreateUPtr<TextureManager>(GraphicsBackendType::OpenGL));
    REQUIRE(textureManager);

    bool ok = false;
    REQUIRE_NOTHROW(ok = textureManager->RemoveTextureAsset(TextureManager::ErrorTextureSID));
    REQUIRE_FALSE(ok);

    Vector<Utils::StringID> sids;
    sids.push_back(TextureManager::ErrorTextureSID);
    REQUIRE_NOTHROW(textureManager->RemoveTexturesAssets(sids));
    const auto texturesCount = textureManager->TexturesAssetsCount();
    REQUIRE(texturesCount == 1UL); // error texture not deleted

    const Assets::TextureAsset* errorTextureAsset = nullptr;
    REQUIRE_NOTHROW(errorTextureAsset = &(textureManager->GetTextureAsset(TextureManager::ErrorTextureSID)));
    REQUIRE(errorTextureAsset->GetStringID() == TextureManager::ErrorTextureSID);

    const Utils::StringID garbageSid = 1234;
    REQUIRE_NOTHROW(errorTextureAsset = &(textureManager->GetTextureAsset(garbageSid)));
    REQUIRE(errorTextureAsset->GetStringID() == TextureManager::ErrorTextureSID); // still error texture

    Filepath garbagePath;
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(TextureManager::ErrorTextureSID, garbagePath));
    REQUIRE_FALSE(ok);
}
//--------------------------------------------------------------------------

TEST_CASE("TextureManager texture functions", "[graphics][texture_manager][texture]")
{
    prepareOpenGLBackend();

    UPtr<TextureManager> textureManager;
    REQUIRE_NOTHROW(textureManager = CreateUPtr<TextureManager>(GraphicsBackendType::OpenGL));
    REQUIRE(textureManager);

    // loading from image
    const auto image = Image(Filepath(KMP_TEST_ICON_PATH), ImageChannels::RGBAlpha);
    const Utils::StringID imageSid = 12345UL;
    bool ok = false;
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(imageSid, image));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL);

    // checking texture properties
    UInt64 textureHandle = 0UL;
    Assets::TextureAsset* textureAsset = nullptr;
    REQUIRE_NOTHROW(textureAsset = &(textureManager->GetTextureAsset(imageSid)));
    REQUIRE(textureAsset);
    REQUIRE(textureAsset->GetStringID() == imageSid);
    void* textureRawHandlePtr = nullptr;
    REQUIRE_NOTHROW(textureRawHandlePtr = textureAsset->GetTexture().GetHandle());
    textureHandle = (reinterpret_cast<UInt64>(textureRawHandlePtr));
    REQUIRE(textureHandle != 0UL);

    // try adding same texture from image
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(imageSid, image));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL);

    // try adding same texture from filepath
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(imageSid, Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL);

    // try remove texture (single sid)
    REQUIRE_NOTHROW(ok = textureManager->RemoveTextureAsset(imageSid));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 1UL);

    // add texture again
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(imageSid, image));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL);

    // try remove texture (invalid sid)
    REQUIRE_NOTHROW(ok = textureManager->RemoveTextureAsset(8888));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL);

    // try remove texture (vector of sids)
    Vector<Utils::StringID> sids;
    sids.push_back(8888);
    REQUIRE_NOTHROW(textureManager->RemoveTexturesAssets(sids));
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL); // not deleted

    sids.push_back(imageSid);
    REQUIRE_NOTHROW(textureManager->RemoveTexturesAssets(sids));
    REQUIRE(textureManager->TexturesAssetsCount() == 1UL); // deleted

    // adding texture from filepath
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(imageSid, Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 2UL);

    // adding texture from non-image filepath (removing first)
    REQUIRE_NOTHROW(ok = textureManager->RemoveTextureAsset(imageSid));
    REQUIRE(ok);
    REQUIRE_NOTHROW(ok = textureManager->CreateTextureAsset(imageSid, Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf")));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesAssetsCount() == 1UL);
}
//--------------------------------------------------------------------------