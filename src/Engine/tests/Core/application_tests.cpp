#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <functional>

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
        TestApplication(const std::string& settingsFilePath, const std::string& defaultSettingsName = "Kmplete_settings.json")
            : Application(settingsFilePath, defaultSettingsName)
        {}

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
}
//--------------------------------------------------------------------------

TEST_CASE("Test application initialize", "[core][application]")
{
    {
        const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>("", "Kmplete_unit_tests_settings.json");

        REQUIRE(application);
        REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());
    }

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_unit_tests_settings.json");
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(settingsPath));
    REQUIRE(Kmplete::Filesystem::PathExists(settingsPath));
}
//--------------------------------------------------------------------------

TEST_CASE("Test application events", "[core][application]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>("", "Kmplete_unit_tests_settings.json");

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