#include "Kmplete/Core/window.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    Window::WindowSettings::WindowSettings(const std::string& name) noexcept
        : name(name)
    {}
    //--------------------------------------------------------------------------

    Window::Window(const Ptr<WindowSettings> settings)
        : _settings(settings)
    {
        if (!settings || settings->name.empty())
        {
            throw std::exception();
        }
    }
    //--------------------------------------------------------------------------

    const std::string& Window::GetName() const
    {
        return _settings->name;
    }
    //--------------------------------------------------------------------------

    std::string Window::ModeToString(Mode mode) noexcept
    {
        switch (mode)
        {
        case FullscreenMode:
            return std::string(FullscreenModeStr);

        case WindowedFullscreenMode:
            return std::string(WindowedFullscreenModeStr);

        case WindowedMode:
            return std::string(WindowedModeStr);

        default:
        {
            KMP_LOG_CORE_WARN("Window: ModeToString unknown mode '{}'", int(mode));
            return std::string("Unknown");
        }
        }
    }
    //--------------------------------------------------------------------------

    Window::Mode Window::StringToMode(const std::string& modeStr) noexcept
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
            KMP_LOG_CORE_WARN("Window: StringToMode unknown mode '{}', return Windowed as default", modeStr);
            return Window::Mode::WindowedMode;
        }
    }
    //--------------------------------------------------------------------------
}