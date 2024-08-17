#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/window.h"

namespace Kmplete
{
    class WindowBackend
    {
    public:
        constexpr static auto MainWindowEntryName = "MainWindow";

    public:
        struct MonitorVideoMode
        {
            int width;
            int height;
            int refreshRate;
        };

    public:
        KMP_NODISCARD KMP_API static Ptr<WindowBackend> Create();

    public:
        KMP_API virtual ~WindowBackend() = default;

        KMP_NODISCARD KMP_API virtual bool Initialize() const = 0;
        KMP_API virtual void Finalize() const = 0;

        KMP_NODISCARD KMP_API virtual Ptr<Window> CreateMainWindow() = 0;

        KMP_NODISCARD KMP_API virtual int GetMonitorCount() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<std::string> GetMonitorNames() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<MonitorVideoMode> GetMonitorVideoModes(int index) const = 0;

        KMP_API virtual void SaveSettings(const Ptr<Settings> settings) const = 0;
        KMP_API virtual void LoadSettings(const Ptr<Settings> settings) = 0;
    };
    //--------------------------------------------------------------------------
}