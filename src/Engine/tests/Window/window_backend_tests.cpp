#include "Kmplete/Window/window_backend.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Window backend monitor functions", "[core][window_backend][monitor]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    const auto monitorsCount = windowBackend->GetMonitorCount();
    REQUIRE(monitorsCount > 0);

    const auto monitorNames = windowBackend->GetMonitorNames();
    REQUIRE(!monitorNames.empty());

    bool monitorNamesNotEmpty = true;
    for (const auto& monitorName : monitorNames)
    {
        if (monitorName.empty())
        {
            monitorNamesNotEmpty = false;
            break;
        }
    }
    REQUIRE(monitorNamesNotEmpty);

    const auto primaryMonitorModes = windowBackend->GetPrimaryMonitorVideoModes();
    REQUIRE(!primaryMonitorModes.empty());

    auto primaryMonitorModesDataValid = true;
    for (const auto& monitorMode : primaryMonitorModes)
    {
        if (monitorMode.width <= 0 || monitorMode.height <= 0 || monitorMode.refreshRate <= 0)
        {
            primaryMonitorModesDataValid = false;
            break;
        }
    }
    REQUIRE(primaryMonitorModesDataValid);

    // assume previous test for primary monitor is ok
    const auto firstMonitorVideoModes = windowBackend->GetMonitorVideoModes(0);
    REQUIRE(!firstMonitorVideoModes.empty());

    auto firstMonitorVideoModesDataValid = true;
    for (const auto& monitorMode : firstMonitorVideoModes)
    {
        if (monitorMode.width <= 0 || monitorMode.height <= 0 || monitorMode.refreshRate <= 0)
        {
            firstMonitorVideoModesDataValid = false;
            break;
        }
    }
    REQUIRE(firstMonitorVideoModesDataValid);

    auto nonExistentMonitorVideoModes = std::vector<Kmplete::WindowBackend::MonitorVideoMode>();
    REQUIRE_NOTHROW(nonExistentMonitorVideoModes = windowBackend->GetMonitorVideoModes(monitorsCount));
    REQUIRE(nonExistentMonitorVideoModes.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Window backend primary monitor DPI scale", "[core][window_backend][monitor]")
{
    const auto windowBackend = Kmplete::WindowBackend::Create();
    REQUIRE(windowBackend);

    auto dpi = 0.0f;
    REQUIRE_NOTHROW(dpi = windowBackend->GetPrimaryMonitorDPIScale());
    REQUIRE(dpi != 0.0f);
}
//--------------------------------------------------------------------------
