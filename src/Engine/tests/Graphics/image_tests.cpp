#include "Kmplete/Graphics/image.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/string_utils.h"

#include <catch2/catch_test_macros.hpp>


using namespace Kmplete;

TEST_CASE("Image from valid filepath", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(Filepath(KMP_TEST_ICON_PATH)));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width != 0);
    REQUIRE(height != 0);
    REQUIRE(channels != 0);
    REQUIRE(pixels != nullptr);

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(Filepath(KMP_TEST_ICON_PATH), ImageChannels::Unknown));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width != 0);
    REQUIRE(height != 0);
    REQUIRE(channels != 0);
    REQUIRE(pixels != nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from invalid filepath", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(Filepath()));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(Filepath(), ImageChannels::Unknown));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from valid filepath but not an image", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    const auto filepath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(filepath));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from valid pixel buffer", "[graphics][image]")
{
    const auto iconBufferSize = 4 * 2 * 4;
    unsigned char iconBuffer[] = {
        /*blue*/ 0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,
        /*red */ 255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255 };

    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(&iconBuffer[0], iconBufferSize, Math::Size2I(4, 2), ImageChannels::RGBAlpha));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 4);
    REQUIRE(height == 2);
    REQUIRE(channels == 4);
    REQUIRE(pixels != nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from valid pixel buffer invalid parameters", "[graphics][image]")
{
    const auto iconBufferSize = 4 * 2 * 4;
    unsigned char iconBuffer[] = {
        /*blue*/ 0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,  0, 0, 255, 255,
        /*red */ 255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255,  255, 0, 0, 255 };

    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    // size bigger than buffer
    REQUIRE_NOTHROW(image = CreateUPtr<Image>(&iconBuffer[0], iconBufferSize, Math::Size2I(6, 4), ImageChannels::RGBAlpha));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);

    // size dimension is zero
    REQUIRE_NOTHROW(image = CreateUPtr<Image>(&iconBuffer[0], iconBufferSize, Math::Size2I(4, 0), ImageChannels::RGBAlpha));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);

    // buffer size is zero
    REQUIRE_NOTHROW(image = CreateUPtr<Image>(&iconBuffer[0], 0, Math::Size2I(4, 2), ImageChannels::RGBAlpha));
    REQUIRE(image);
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from nullptr pixel buffer", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(nullptr, 32, Math::Size2I(4, 2), ImageChannels::RGBAlpha));
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from valid image file buffer", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    const auto filepath = Filepath(KMP_TEST_ICON_PATH);
    const auto iconFileBinaryBuffer = Filesystem::ReadFileAsBinary(filepath);
    REQUIRE(!iconFileBinaryBuffer.empty());

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(iconFileBinaryBuffer.data(), int(iconFileBinaryBuffer.size()), ImageChannels::RGB));
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width != 0);
    REQUIRE(height != 0);
    REQUIRE(channels != 0);
    REQUIRE(pixels != nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from valid file buffer not an image", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    const auto filepath = Utils::Concatenate(KMP_FONTS_FOLDER, "OpenSans-Regular.ttf");
    const auto iconFileBinaryBuffer = Filesystem::ReadFileAsBinary(filepath);
    REQUIRE(!iconFileBinaryBuffer.empty());

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(iconFileBinaryBuffer.data(), int(iconFileBinaryBuffer.size()), ImageChannels::RGB));
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Image from nullptr file buffer", "[graphics][image]")
{
    UPtr<Image> image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;
    UByte* pixels = nullptr;

    REQUIRE_NOTHROW(image = CreateUPtr<Image>(nullptr, 32, ImageChannels::RGBAlpha));
    REQUIRE_NOTHROW(width = image->GetWidth());
    REQUIRE_NOTHROW(height = image->GetHeight());
    REQUIRE_NOTHROW(channels = image->GetChannels());
    REQUIRE_NOTHROW(pixels = image->GetPixels());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(channels == 0);
    REQUIRE(pixels == nullptr);
}
//--------------------------------------------------------------------------