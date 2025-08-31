#include "Kmplete/Core/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"

namespace Kmplete
{
    Window::WindowSettings::WindowSettings(const String& name) noexcept
        : name(name)
    {}
    //--------------------------------------------------------------------------

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

    const String& Window::GetName() const
    {
        return _settings.name;
    }
    //--------------------------------------------------------------------------

    String Window::ModeToString(Mode mode) noexcept
    {
        KMP_PROFILE_FUNCTION();

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
            KMP_LOG_WARN("ModeToString unknown mode '{}'", int(mode));
            return String("Unknown");
        }
        }
    }
    //--------------------------------------------------------------------------

    Window::Mode Window::StringToMode(const String& modeStr) noexcept
    {
        KMP_PROFILE_FUNCTION();

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
            KMP_LOG_WARN("StringToMode unknown mode '{}', return Windowed as default", modeStr);
            return Window::Mode::WindowedMode;
        }
    }
    //--------------------------------------------------------------------------
}