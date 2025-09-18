#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Log/log.h"

namespace Kmplete
{
    Window::Window(WindowSettings& settings)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("Window::Window(WindowSettings&)")
          _settings(settings)
    {
        if (settings.name.empty())
        {
            KMP_PROFILE_CONSTRUCTOR_END()
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

    String Window::ScreenModeToString(ScreenMode screenMode) noexcept
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

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


    Window::WindowSettings::WindowSettings(const String& name) noexcept
        : name(name)
    {}
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
        settings.SaveBool(ResizableStr, resizable);
        settings.SaveBool(DecoratedStr, decorated);
        settings.SaveBool(AlwaysOnTopStr, alwaysOnTop);
    }
    //--------------------------------------------------------------------------

    void Window::WindowSettings::LoadSettings(SettingsDocument& settings)
    {
        position.x = settings.GetInt(XStr, DefaultX);
        position.y = settings.GetInt(YStr, DefaultY);
        size.x = settings.GetInt(WidthStr, DefaultWidth);
        size.y = settings.GetInt(HeightStr, DefaultHeight);
        windowedSize.x = settings.GetInt(WindowedWidthStr, DefaultWidth);
        windowedSize.y = settings.GetInt(WindowedHeightStr, DefaultHeight);
        screenMode = StringToScreenMode(settings.GetString(ScreenModeStr, WindowedModeStr));
        vSync = settings.GetBool(VSyncStr, true);
        updateContinuously = settings.GetBool(UpdateContinuouslyStr, true);
        resizable = settings.GetBool(ResizableStr, true);
        decorated = settings.GetBool(DecoratedStr, true);
        alwaysOnTop = settings.GetBool(AlwaysOnTopStr, false);
    }
    //--------------------------------------------------------------------------
}