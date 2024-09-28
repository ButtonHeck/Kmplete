#include "Kmplete/Core/window.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    Window::WindowSettings::WindowSettings(const std::string& name)
        : name(name)
    {
        Log::CoreTrace("WindowSettings: created '{}'", name);
    }
    //--------------------------------------------------------------------------

    Window::Window(const Ptr<WindowSettings> settings)
        : _settings(settings)
    {
        KMP_CHECK(settings).Exception();
        KMP_CHECK(!settings->name.empty()).Exception();
    }
    //--------------------------------------------------------------------------

    const std::string& Window::GetName() const
    {
        return _settings->name;
    }
    //--------------------------------------------------------------------------

    std::string Window::ModeToString(Mode mode) KMP_NOEXCEPT
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
            Log::CoreWarn("Window: ModeToString unknown mode '{}'", int(mode));
            return std::string("Unknown");
        }
        }
    }
    //--------------------------------------------------------------------------

    Window::Mode Window::StringToMode(const std::string& modeStr) KMP_NOEXCEPT
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
            Log::CoreWarn("Window: StringToMode unknown mode '{}', return Windowed as default", modeStr);
            return Window::Mode::WindowedMode;
        }
    }
    //--------------------------------------------------------------------------
}