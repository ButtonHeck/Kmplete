#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto NameStr = "Name";
    static constexpr auto XStr = "X";
    static constexpr auto YStr = "Y";
    static constexpr auto WidthStr = "Width";
    static constexpr auto HeightStr = "Height";
    static constexpr auto WindowedWidthStr = "WindowedWidth";
    static constexpr auto WindowedHeightStr = "WindowedHeight";
    static constexpr auto ScreenModeStr = "ScreenMode";
    static constexpr auto VSyncStr = "VSync";
    static constexpr auto UpdateContinuouslyStr = "UpdateContinuously";
    static constexpr auto AlwaysOnTopStr = "AlwaysOnTop";


    Window::Window(WindowSettings& settings, Graphics::GraphicsBackendType graphicsBackendType)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _settings(settings)
        , _graphicsBackendType(graphicsBackendType)
    {
        if (settings.name.empty())
        {
            KMP_LOG_ERROR("cannot create window with empty name");
            throw std::exception();
        }

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    const String& Window::GetName() const noexcept
    {
        return _settings.name;
    }
    //--------------------------------------------------------------------------

    Graphics::GraphicsBackendType Window::GetGraphicsBackendType() const noexcept
    {
        return _graphicsBackendType;
    }
    //--------------------------------------------------------------------------

    String Window::ScreenModeToString(ScreenMode screenMode) noexcept
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        switch (screenMode)
        {
        case ScreenMode::Fullscreen:
            return String(FullscreenModeStr);

        case ScreenMode::WindowedFullscreen:
            return String(WindowedFullscreenModeStr);

        case ScreenMode::Windowed:
            return String(WindowedModeStr);

        default:
        {
            KMP_LOG_WARN("ScreenModeToString unknown mode '{}'", int(screenMode));
            return String("Unknown");
        }
        }
    }
    //--------------------------------------------------------------------------

    Window::ScreenMode Window::StringToScreenMode(const String& screenModeStr) noexcept
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        if (screenModeStr == FullscreenModeStr)
        {
            return Window::ScreenMode::Fullscreen;
        }
        else if (screenModeStr == WindowedFullscreenModeStr)
        {
            return Window::ScreenMode::WindowedFullscreen;
        }
        else if (screenModeStr == WindowedModeStr)
        {
            return Window::ScreenMode::Windowed;
        }
        else
        {
            KMP_LOG_WARN("StringToScreenMode unknown mode '{}', return Windowed as default", screenModeStr);
            return Window::ScreenMode::Windowed;
        }
    }
    //--------------------------------------------------------------------------

    bool Window::IsFullscreen() const noexcept
    {
        return _settings.IsFullscreen();
    }
    //--------------------------------------------------------------------------

    bool Window::IsWindowed() const noexcept
    {
        return _settings.IsWindowed();
    }
    //--------------------------------------------------------------------------

    bool Window::IsWindowedFullscreen() const noexcept
    {
        return _settings.IsWindowedFullscreen();
    }
    //--------------------------------------------------------------------------


    bool Window::WindowSettings::IsFullscreen() const noexcept
    {
        return screenMode == Window::ScreenMode::Fullscreen;
    }
    //--------------------------------------------------------------------------

    bool Window::WindowSettings::IsWindowed() const noexcept
    {
        return screenMode == Window::ScreenMode::Windowed;
    }
    //--------------------------------------------------------------------------

    bool Window::WindowSettings::IsWindowedFullscreen() const noexcept
    {
        return screenMode == Window::ScreenMode::WindowedFullscreen;
    }
    //--------------------------------------------------------------------------

    void Window::WindowSettings::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.SaveString(NameStr, name);
        settings.SaveInt(XStr, position.x);
        settings.SaveInt(YStr, position.y);
        settings.SaveInt(WidthStr, size.x);
        settings.SaveInt(HeightStr, size.y);
        settings.SaveInt(WindowedWidthStr, windowedSize.x);
        settings.SaveInt(WindowedHeightStr, windowedSize.y);
        settings.SaveString(ScreenModeStr, ScreenModeToString(screenMode));
        settings.SaveBool(VSyncStr, vSync);
        settings.SaveBool(UpdateContinuouslyStr, updateContinuously);
        settings.SaveBool(AlwaysOnTopStr, alwaysOnTop);
    }
    //--------------------------------------------------------------------------

    void Window::WindowSettings::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        name = settings.GetString(NameStr, "");
        position.x = settings.GetInt(XStr, DefaultX);
        position.y = settings.GetInt(YStr, DefaultY);
        size.x = settings.GetInt(WidthStr, DefaultWidth);
        size.y = settings.GetInt(HeightStr, DefaultHeight);
        windowedSize.x = settings.GetInt(WindowedWidthStr, DefaultWidth);
        windowedSize.y = settings.GetInt(WindowedHeightStr, DefaultHeight);
        screenMode = StringToScreenMode(settings.GetString(ScreenModeStr, WindowedModeStr));
        vSync = settings.GetBool(VSyncStr, true);
        updateContinuously = settings.GetBool(UpdateContinuouslyStr, true);
        alwaysOnTop = settings.GetBool(AlwaysOnTopStr, false);
    }
    //--------------------------------------------------------------------------
}