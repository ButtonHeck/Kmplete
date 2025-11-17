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

        bool FontAssetManager::CreateAsset(Utils::StringID fontSid, BinaryBuffer&& fontData)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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
        }
        //--------------------------------------------------------------------------

        bool FontAssetManager::CreateAsset(Utils::StringID fontSid, const Filepath& filepath)
        {
            return CreateAsset(fontSid, Filesystem::ReadFileAsBinary(filepath));
        }
        //--------------------------------------------------------------------------

        const Assets::FontAsset& FontAssetManager::GetAsset(Utils::StringID fontSid) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            if (!_fonts.contains(fontSid))
            {
                KMP_LOG_ERROR("font '{}' not found", fontSid);
                return *_fonts.at(DefaultFontSID);
            }

            return *_fonts.at(fontSid);
        }
        //--------------------------------------------------------------------------

        Assets::FontAsset& FontAssetManager::GetAsset(Utils::StringID fontSid)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            if (!_fonts.contains(fontSid))
            {
                KMP_LOG_ERROR("font '{}' not found", fontSid);
                return *_fonts.at(DefaultFontSID);
            }

            return *_fonts.at(fontSid);
        }
        //--------------------------------------------------------------------------

        void FontAssetManager::RemoveAssets(const Vector<Utils::StringID>& sids)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto ok = true;
            for (const auto& sid : sids)
            {
                ok &= RemoveAsset(sid);
            }

            if (!ok)
            {
                KMP_LOG_WARN("some fonts were not removed");
            }
        }
        //--------------------------------------------------------------------------

        bool FontAssetManager::RemoveAsset(Utils::StringID sid)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

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
        }
        //--------------------------------------------------------------------------

        UInt64 FontAssetManager::GetAssetsCount() const noexcept
        {
            return _fonts.size();
        }
        //--------------------------------------------------------------------------

        void FontAssetManager::_Initialize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

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
        }
        //--------------------------------------------------------------------------

        void FontAssetManager::_Finalize()
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

        bool FontAssetManager::_CreateDefaultFontAsset()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

#if defined (KMP_PLATFORM_WINDOWS)
            if (_fonts.contains(DefaultFontSID))
            {
                KMP_LOG_WARN("default font already created");
                return false;
            }

            auto hdc = GetDC(NULL);
            const auto fontDescriptor = CreateFont(
                18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

            SelectObject(hdc, fontDescriptor);
            const auto fontDataSize = GetFontData(hdc, 0, 0, NULL, 0);

            BinaryBuffer fontData;
            if (fontDataSize != GDI_ERROR)
            {
                fontData = BinaryBuffer(fontDataSize);
                GetFontData(hdc, 0, 0, fontData.data(), fontDataSize);
            }

            DeleteObject(fontDescriptor);
            ReleaseDC(NULL, hdc);

            return _AddFontToStorage(DefaultFontSID, std::move(fontData));
#else
            //TODO: make similar in Linux (at least)
            return true;
#endif
        }
        //--------------------------------------------------------------------------

        bool FontAssetManager::_AddFontToStorage(Utils::StringID sid, BinaryBuffer&& fontData)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto [iterator, hasEmplaced] = _fonts.emplace(sid, CreateUPtr<Assets::FontAsset>(sid, *_freetypeLibInstance, std::move(fontData)));
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------
    }
}