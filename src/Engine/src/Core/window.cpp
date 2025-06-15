#include "Kmplete/Core/window.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    Window::WindowSettings::WindowSettings(const String& name) noexcept
        : name(name)
    {}
    //--------------------------------------------------------------------------

    Window::Window(WindowSettings& settings)
        : _settings(settings)
    {
        if (settings.name.empty())
        {
            throw std::exception();
        }
    }
    //--------------------------------------------------------------------------

    const String& Window::GetName() const
    {
        return _settings.name;
    }
    //--------------------------------------------------------------------------

    String Window::ModeToString(Mode mode) noexcept
    {
        switch (mode)
        {
        case FullscreenMode:
            return String(FullscreenModeStr);

        case WindowedFullscreenMode:
            return String(WindowedFullscreenModeStr);

        case WindowedMode:
            return String(WindowedModeStr);

        default:
        {
            KMP_LOG_WARN("Window: ModeToString unknown mode '{}'", int(mode));
            return String("Unknown");
        }
        }
    }
    //--------------------------------------------------------------------------

    Window::Mode Window::StringToMode(const String& modeStr) noexcept
    {
        if (modeStr == FullscreenModeStr)
        {
            return Window::Mode::FullscreenMode;
        }
        else if (modeStr == WindowedFullscreenModeStr)
        {
            return Window::Mode::WindowedFullscreenMode;
        }
        else if (modeStr == WindowedModeStr)
        {
            return Window::Mode::WindowedMode;
        }
        else
        {
            KMP_LOG_WARN("Window: StringToMode unknown mode '{}', return Windowed as default", modeStr);
            return Window::Mode::WindowedMode;
        }
    }
    //--------------------------------------------------------------------------
}