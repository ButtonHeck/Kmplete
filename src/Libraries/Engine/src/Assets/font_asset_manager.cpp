#include "Kmplete/Assets/font_asset_manager.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>

#if defined (KMP_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined (KMP_PLATFORM_LINUX)
    #include <fontconfig/fontconfig.h>
#endif


namespace Kmplete
{
    namespace Assets
    {
        FontAssetManager::FontAssetManager()
            : _freetypeLibInstance(nullptr)
        {
            _Initialize();
        }
        //--------------------------------------------------------------------------

        FontAssetManager::~FontAssetManager()
        {
            _Finalize();
        }
        //--------------------------------------------------------------------------

        bool FontAssetManager::CreateAsset(StringID fontSid, BinaryBuffer&& fontData) KMP_PROFILING(ProfileLevelImportant)
        {
            if (fontSid == DefaultFontSID)
            {
                KMP_LOG_ERROR("cannot create font with zero id");
                return false;
            }

            if (_fonts.contains(fontSid))
            {
                KMP_LOG_ERROR("already contains font with sid '{}'", fontSid);
                return false;
            }

            return _AddFontToStorage(fontSid, std::move(fontData));
        }}
        //--------------------------------------------------------------------------

        bool FontAssetManager::CreateAsset(StringID fontSid, const Filepath& filepath)
        {
            return CreateAsset(fontSid, Filesystem::ReadFileAsBinary(filepath));
        }
        //--------------------------------------------------------------------------

        const Assets::FontAsset& FontAssetManager::GetAsset(StringID fontSid) const KMP_PROFILING(ProfileLevelMinor)
        {
            if (!_fonts.contains(fontSid))
            {
                KMP_LOG_ERROR("font '{}' not found", fontSid);
                return *_fonts.at(DefaultFontSID);
            }

            return *_fonts.at(fontSid);
        }}
        //--------------------------------------------------------------------------

        Assets::FontAsset& FontAssetManager::GetAsset(StringID fontSid) KMP_PROFILING(ProfileLevelMinor)
        {
            if (!_fonts.contains(fontSid))
            {
                KMP_LOG_ERROR("font '{}' not found", fontSid);
                return *_fonts.at(DefaultFontSID);
            }

            return *_fonts.at(fontSid);
        }}
        //--------------------------------------------------------------------------

        void FontAssetManager::RemoveAssets(const Vector<StringID>& sids) KMP_PROFILING(ProfileLevelImportant)
        {
            auto ok = true;
            for (const auto& sid : sids)
            {
                ok &= RemoveAsset(sid);
            }

            if (!ok)
            {
                KMP_LOG_WARN("some fonts were not removed");
            }
        }}
        //--------------------------------------------------------------------------

        bool FontAssetManager::RemoveAsset(StringID sid) KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (sid == DefaultFontSID)
            {
                KMP_LOG_WARN("cannot remove font with reserved sid 0");
                return false;
            }

            if (_fonts.erase(sid) == 0)
            {
                KMP_LOG_WARN("not found or failed to remove font with sid '{}'", sid);
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------

        UInt64 FontAssetManager::GetAssetsCount() const noexcept
        {
            return _fonts.size();
        }
        //--------------------------------------------------------------------------

        void FontAssetManager::_Initialize() KMP_PROFILING(ProfileLevelAlways)
        {
            const auto freetypeInitError = FT_Init_FreeType(&_freetypeLibInstance);
            if (freetypeInitError)
            {
                KMP_LOG_CRITICAL("failed to initialize FreeType library instance");
                throw std::runtime_error("FontAssetManager: failed to initialize FreeType library instance");
            }

            if (!_CreateDefaultFontAsset())
            {
                KMP_LOG_CRITICAL("default font loading failed");
                throw std::runtime_error("FontAssetManager: default font loading failed");
            }

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
            FT_Int freetypeVersionMajor = 0;
            FT_Int freetypeVersionMinor = 0;
            FT_Int freetypeVersionPatch = 0;
            FT_Library_Version(_freetypeLibInstance, &freetypeVersionMajor, &freetypeVersionMinor, &freetypeVersionPatch);
            KMP_LOG_INFO("use FreeType version {}.{}.{}", freetypeVersionMajor, freetypeVersionMinor, freetypeVersionPatch);
#endif
        }}
        //--------------------------------------------------------------------------

        void FontAssetManager::_Finalize() KMP_PROFILING(ProfileLevelAlways)
        {
            _fonts.clear();

            const auto freetypeDoneError = FT_Done_FreeType(_freetypeLibInstance);
            if (freetypeDoneError)
            {
                KMP_LOG_ERROR("failed to shutdown FreeType library instance");
            }
        }}
        //--------------------------------------------------------------------------

        bool FontAssetManager::_CreateDefaultFontAsset() KMP_PROFILING(ProfileLevelImportant)
        {
            static constexpr auto DefaultFontSize = 18;

            if (_fonts.contains(DefaultFontSID))
            {
                KMP_LOG_WARN("default font already created");
                return false;
            }

#if defined (KMP_PLATFORM_WINDOWS)
            auto hdc = GetDC(NULL);
            if (!hdc)
            {
                KMP_LOG_ERROR("failed to get device context");
                return false;
            }

            const auto fontDescriptor = CreateFont(
                DefaultFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

            if (!fontDescriptor)
            {
                KMP_LOG_ERROR("failed to create default font descriptor");
                ReleaseDC(NULL, hdc);
                return false;
            }

            auto cleanupGDI = [&]() { 
                DeleteObject(fontDescriptor); 
                ReleaseDC(NULL, hdc);
            };

            SelectObject(hdc, fontDescriptor);
            const auto fontDataSize = GetFontData(hdc, 0, 0, NULL, 0);

            if (fontDataSize == GDI_ERROR)
            {
                KMP_LOG_ERROR("failed to get default font data size");
                cleanupGDI();
                return false;
            }

            BinaryBuffer fontData(fontDataSize);
            if (GetFontData(hdc, 0, 0, fontData.data(), fontDataSize) == GDI_ERROR)
            {
                KMP_LOG_ERROR("failed to get default font data from GDI");
                cleanupGDI();
                return false;
            }

            cleanupGDI();

            return _AddFontToStorage(DefaultFontSID, std::move(fontData));
#else
            if (!FcInit())
            {
                KMP_LOG_ERROR("failed to initialize Fontconfig library");
                return false;
            }

            auto* pattern = FcPatternCreate();
            FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)"ubuntu");
            FcPatternAddInteger(pattern, FC_SIZE, DefaultFontSize);

            auto* config = FcConfigGetCurrent();

            FcResult result;
            auto* match = FcFontMatch(config, pattern, &result);
            auto cleanupFc = [&]() { 
                if (pattern)
                {
                    FcPatternDestroy(pattern);
                }
                if (match)
                {
                    FcPatternDestroy(match);
                }
                FcFini();
            };

            if (!match)
            {
                KMP_LOG_ERROR("failed to find default 'ubuntu' font");
                cleanupFc();
                return false;
            }

            FcChar8* fcFilepath;
            if (FcPatternGetString(match, FC_FILE, 0, &fcFilepath) != FcResultMatch)
            {
                KMP_LOG_ERROR("failed to get default font path");
                cleanupFc();
                return false;
            }

            const auto filepath = String(reinterpret_cast<char*>(fcFilepath));
            BinaryBuffer fontData = Filesystem::ReadFileAsBinary(filepath);
            if (fontData.empty())
            {
                KMP_LOG_ERROR("failed to get font buffer");
                cleanupFc();
                return false;
            }

            cleanupFc();

            return _AddFontToStorage(DefaultFontSID, std::move(fontData));
#endif
        }}
        //--------------------------------------------------------------------------

        bool FontAssetManager::_AddFontToStorage(StringID sid, BinaryBuffer&& fontData) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto [iterator, hasEmplaced] = _fonts.emplace(sid, CreateUPtr<Assets::FontAsset>(sid, *_freetypeLibInstance, std::move(fontData)));
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------
    }
}