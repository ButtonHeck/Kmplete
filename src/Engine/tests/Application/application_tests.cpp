#include "Kmplete/Application/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <thread>

namespace Kmplete
{
    using EventCallbackFn = std::function<void(Event&)>;

    class EventSender
    {
    public:
        void SetCallbackFn(const EventCallbackFn& fn) { _callback = fn; }

        void SendKeyPressEvent() const
        {
            KeyPressEvent evt(Kmplete::Key::A, 0);
            _callback(evt);
        }

        void SendKeyReleaseEvent() const
        {
            KeyReleaseEvent evt(Kmplete::Key::A);
            _callback(evt);
        }

        void SendKeyCharEvent() const
        {
            KeyCharEvent evt(Kmplete::Key::A);
            _callback(evt);
        }

    private:
        EventCallbackFn _callback = nullptr;
    };

    class TestApplication : public Application
    {
    public:
        TestApplication(const ApplicationParameters& applicationParameters)
            : Application(applicationParameters)
        {
            _localizationManager->SetLocale("ru_RU.UTF8");
        }

        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override 
        { 
            return std::string("TestApplication"); 
        }

        void SetSender(Ptr<EventSender> sender)
        {
            _sender = sender;
            _sender->SetCallbackFn(KMP_BIND(TestApplication::OnEvent));
        }

        bool IsKeyPressEventInvoked() const  { return _keyPressEventInvoked; }
        bool IsKeyReleaseEventInvoked() const  { return _keyReleaseEventInvoked; }
        bool IsKeyCharEventInvoked() const  { return _keyCharEventInvoked; }
        bool IsApplicationKeyPressEventInvoked() const { return _applicationKeyPressEventInvoked; }
        bool IsApplicationKeyReleaseEventInvoked() const { return _applicationKeyReleaseEventInvoked; }
        bool IsApplicationKeyCharEventInvoked() const { return _applicationKeyCharEventInvoked; }

        void Run() override {}

        void SaveSettings(const Path& path = Path()) const override
        {
            Application::SaveSettings(path);
        }

        void LoadSettings(const Path& path = Path()) override
        {
            Application::LoadSettings(path);
        }

    protected:
        void OnEvent(Event& event) override
        {
            EventDispatcher dispatcher(event);

            _applicationKeyPressEventInvoked = dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(TestApplication::OnKeyPressEvent));
            _applicationKeyReleaseEventInvoked = dispatcher.Dispatch<KeyReleaseEvent>(KMP_BIND(TestApplication::OnKeyReleaseEvent));
            _applicationKeyCharEventInvoked = dispatcher.Dispatch<KeyCharEvent>(KMP_BIND(TestApplication::OnKeyCharEvent));
        }

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent& evt) override { _keyPressEventInvoked = true; return Application::OnKeyPressEvent(evt); }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent& evt) override { _keyReleaseEventInvoked = true; return Application::OnKeyReleaseEvent(evt); }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent& evt) override { _keyCharEventInvoked = true; return Application::OnKeyCharEvent(evt); }

    private:
        Ptr<EventSender> _sender = nullptr;
        bool _keyPressEventInvoked = false;
        bool _keyReleaseEventInvoked = false;
        bool _keyCharEventInvoked = false;
        bool _applicationKeyPressEventInvoked = false;
        bool _applicationKeyReleaseEventInvoked = false;
        bool _applicationKeyCharEventInvoked = false;
    };

    class MetricsTestApplication : public Application
    {
    public:
        MetricsTestApplication(const ApplicationParameters& applicationParameters)
            : Application(applicationParameters)
        {}

        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override
        {
            return std::string("MetricsTestApplication");
        }

        void Run() override {}
        bool UpdateMetrics() { return _systemMetricsManager->Update(); }

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
        const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>(Kmplete::ApplicationParameters("", "Kmplete_unit_tests_settings.json"));

        REQUIRE(application);
        REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());
    }

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_unit_tests_settings.json");
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(settingsPath));
    REQUIRE(Kmplete::Filesystem::PathExists(settingsPath));
}
//--------------------------------------------------------------------------

TEST_CASE("Test application events", "[application]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>(Kmplete::ApplicationParameters("", "Kmplete_unit_tests_settings.json"));

    REQUIRE(application);

    const auto eventSender = Kmplete::CreatePtr<Kmplete::EventSender>();
    application->SetSender(eventSender);

    REQUIRE_FALSE(application->IsKeyPressEventInvoked());
    REQUIRE_FALSE(application->IsKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->IsKeyCharEventInvoked());
    REQUIRE_FALSE(application->IsApplicationKeyPressEventInvoked());
    REQUIRE_FALSE(application->IsApplicationKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->IsApplicationKeyCharEventInvoked());

    eventSender->SendKeyPressEvent();
    REQUIRE(application->IsApplicationKeyPressEventInvoked());
    REQUIRE(application->IsKeyPressEventInvoked());

    eventSender->SendKeyReleaseEvent();
    REQUIRE(application->IsApplicationKeyReleaseEventInvoked());
    REQUIRE(application->IsKeyReleaseEventInvoked());

    eventSender->SendKeyCharEvent();
    REQUIRE(application->IsApplicationKeyCharEventInvoked());
    REQUIRE(application->IsKeyCharEventInvoked());
}
//--------------------------------------------------------------------------

TEST_CASE("Test application runtime settings save", "[application]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>(Kmplete::ApplicationParameters("", "Kmplete_unit_tests_settings.json"));

    REQUIRE(application);

    const auto newSettingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_unit_test_settings_runtime.json");
    application->SaveSettings(newSettingsPath);

    REQUIRE(Kmplete::Filesystem::PathExists(newSettingsPath));
}
//--------------------------------------------------------------------------

TEST_CASE("Test application runtime settings load", "[application]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>(Kmplete::ApplicationParameters("", "Kmplete_unit_tests_settings.json"));

    REQUIRE(application);

    application->LoadSettings();
    SUCCEED(); // just check succeed build and no exceptions
}
//--------------------------------------------------------------------------

TEST_CASE("Test application metrics update", "[application][metrics]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::MetricsTestApplication>(Kmplete::ApplicationParameters("", "Kmplete_unit_tests_settings.json"));

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