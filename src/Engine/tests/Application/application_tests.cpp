#include "Kmplete/Application/application.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <thread>


namespace Kmplete
{
    class TestApplication : public Application
    {
    public:
        TestApplication(const ApplicationParameters& parameters)
            : Application(parameters)
        {}

        void Run() override {}

    private:
    };

    class MetricsTestApplication : public Application
    {
    public:
        MetricsTestApplication(const ApplicationParameters& parameters)
            : Application(parameters)
        {}

        void Run() override {}
        bool UpdateMetrics() { return _systemMetricsManager->Update(SystemMetricsManager::SystemMetricsUpdateMode::Everything); }

        bool IsMetricsNumProcessorsOk() const { return _systemMetricsManager->GetMetrics().numProcessors > 0; }
        bool IsMetricsNumThreadsOk() const { return _systemMetricsManager->GetMetrics().numThreads > 0; }
        bool IsMetricsCPUUsageOk() const { return _systemMetricsManager->GetMetrics().cpuUsagePercent >= 0.0f; }
        bool IsMetricsTotalPhysicalMemoryOk() const { return _systemMetricsManager->GetMetrics().totalPhysicalMemoryMib > 0.0f; }
        bool IsMetricsPhysicalMemoryUsedOk() const { return _systemMetricsManager->GetMetrics().physicalMemoryUsedMib > 0.0f; }
        bool IsMetricsTotalVirtualMemoryOk() const { return _systemMetricsManager->GetMetrics().totalVirtualMemoryMib > 0.0f; }
        bool IsMetricsVirtualMemoryUsedOk() const { return _systemMetricsManager->GetMetrics().virtualMemoryUsedMib > 0.0f; }
    };
}
//--------------------------------------------------------------------------

TEST_CASE("Test application initialize", "[application]")
{
    {
        const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>(Kmplete::ApplicationParameters("TestApplication", "", "Kmplete_unit_tests_settings.json"));

        REQUIRE(application);
        REQUIRE(!Kmplete::Filesystem::GetCurrentFilepath().empty());
    }

    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append("Kmplete_unit_tests_settings.json");
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(settingsFilepath));
    REQUIRE(Kmplete::Filesystem::FilepathExists(settingsFilepath));
}
//--------------------------------------------------------------------------

TEST_CASE("Test application name", "[application]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::MetricsTestApplication>(Kmplete::ApplicationParameters("MetricsTestApplication", "", "Kmplete_unit_tests_settings.json"));
    REQUIRE(application);
    Kmplete::String appName = "";
    REQUIRE_NOTHROW(appName = application->GetApplicationName());
    REQUIRE(!appName.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Test application metrics update", "[application][metrics]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::MetricsTestApplication>(Kmplete::ApplicationParameters("MetricsTestApplication", "", "Kmplete_unit_tests_settings.json"));

    REQUIRE(application);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    REQUIRE(application->UpdateMetrics());
    REQUIRE(application->IsMetricsNumProcessorsOk());
    REQUIRE(application->IsMetricsNumThreadsOk());
    REQUIRE(application->IsMetricsCPUUsageOk());
    REQUIRE(application->IsMetricsPhysicalMemoryUsedOk());
    REQUIRE(application->IsMetricsTotalPhysicalMemoryOk());
    REQUIRE(application->IsMetricsTotalVirtualMemoryOk());
    REQUIRE(application->IsMetricsVirtualMemoryUsedOk());
}
//--------------------------------------------------------------------------