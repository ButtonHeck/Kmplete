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
    REQUIRE_NOTHROW(ok = textureManager->RemoveTexture(TextureManager::ErrorTextureSID));
    REQUIRE_FALSE(ok);

    Vector<Utils::StringID> sids;
    sids.push_back(TextureManager::ErrorTextureSID);
    REQUIRE_NOTHROW(textureManager->RemoveTextures(sids));
    const auto texturesCount = textureManager->TexturesCount();
    REQUIRE(texturesCount == 1UL); // error texture not deleted

    const Texture* errorTexture = nullptr;
    REQUIRE_NOTHROW(errorTexture = &(textureManager->GetTexture(TextureManager::ErrorTextureSID)));
    REQUIRE(errorTexture->GetStringID() == TextureManager::ErrorTextureSID);

    const Utils::StringID garbageSid = 1234;
    REQUIRE_NOTHROW(errorTexture = &(textureManager->GetTexture(garbageSid)));
    REQUIRE(errorTexture->GetStringID() == TextureManager::ErrorTextureSID); // still error texture

    BinaryBuffer garbageBuffer;
    Image garbageImage(garbageBuffer.data(), 0, ImageChannels::RGB);
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(TextureManager::ErrorTextureSID, garbageImage));
    REQUIRE_FALSE(ok);

    Filepath garbagePath;
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(TextureManager::ErrorTextureSID, garbagePath));
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
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(imageSid, image));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesCount() == 2UL);

    // checking texture properties
    UInt64 textureHandle = 0UL;
    Texture* texture = nullptr;
    REQUIRE_NOTHROW(texture = &(textureManager->GetTexture(imageSid)));
    REQUIRE(texture);
    REQUIRE(texture->GetStringID() == imageSid);
    void* textureRawHandlePtr = nullptr;
    REQUIRE_NOTHROW(textureRawHandlePtr = texture->GetHandle());
    textureHandle = (reinterpret_cast<UInt64>(textureRawHandlePtr));
    REQUIRE(textureHandle != 0UL);

    // try adding same texture from image
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(imageSid, image));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesCount() == 2UL);

    // try adding same texture from filepath
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(imageSid, Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesCount() == 2UL);

    // try remove texture (single sid)
    REQUIRE_NOTHROW(ok = textureManager->RemoveTexture(imageSid));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesCount() == 1UL);

    // add texture again
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(imageSid, image));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesCount() == 2UL);

    // try remove texture (invalid sid)
    REQUIRE_NOTHROW(ok = textureManager->RemoveTexture(8888));
    REQUIRE_FALSE(ok);
    REQUIRE(textureManager->TexturesCount() == 2UL);

    // try remove texture (vector of sids)
    Vector<Utils::StringID> sids;
    sids.push_back(8888);
    REQUIRE_NOTHROW(textureManager->RemoveTextures(sids));
    REQUIRE(textureManager->TexturesCount() == 2UL); // not deleted

    sids.push_back(imageSid);
    REQUIRE_NOTHROW(textureManager->RemoveTextures(sids));
    REQUIRE(textureManager->TexturesCount() == 1UL); // deleted

    // adding texture from filepath
    REQUIRE_NOTHROW(ok = textureManager->CreateTexture(imageSid, Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE(ok);
    REQUIRE(textureManager->TexturesCount() == 2UL);
}
//--------------------------------------------------------------------------