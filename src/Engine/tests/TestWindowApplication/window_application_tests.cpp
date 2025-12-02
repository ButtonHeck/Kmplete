#include "shared_state.h"
#include "test_frame_listeners.h"
#include "test_main_frame_listener.h"

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Assets/font_asset_manager.h"
#include "Kmplete/Assets/assets_manager.h"

#include <catch2/catch_test_macros.hpp>


namespace Kmplete
{
    class TestWindowApplication : public WindowApplication
    {
    public:
        TestWindowApplication(const WindowApplicationParameters& parameters)
            : WindowApplication(parameters)
            , mainFrameListener(nullptr)
            , _mainWindow(_windowBackend->GetMainWindow())
            , _frameListener1(nullptr)
            , _frameListener2(nullptr)
            , _frameListener3(nullptr)
            , _frameListener4(nullptr)
            , _frameListenersCountOk(false)
        {
            Initialize();
        }

        void Initialize()
        {
            _frameListener1.reset(new TestFrameListener1(*_frameListenerManager.get(), _sharedState));
            _frameListener2.reset(new TestFrameListener2(*_frameListenerManager.get(), _sharedState));
            _frameListener3.reset(new TestFrameListener3(*_frameListenerManager.get(), _sharedState));
            _frameListener4.reset(new TestFrameListener4(*_frameListenerManager.get(), _sharedState));
            mainFrameListener.reset(new TestMainFrameListener(*_frameListenerManager.get(), _sharedState, _mainWindow, _assetsManager.get(), _graphicsBackend.get(), _windowBackend.get()));

            _frameListenersCountOk = _frameListenerManager->FrameListenersCount() == 5;
            _frameListenerManager->SetCreateDeleteCommandBufferHandler(KMP_BIND(TestWindowApplication::FrameListenerCommandBufferHandlerFunction));
        }

        void FrameListenerCommandBufferHandlerFunction(const FrameListenerCommandBuffer& commandBuffer)
        {
            for (const auto& command : commandBuffer)
            {
                if (command.code == FrameListenerCommandCode::Delete && command.sid == 1ULL)
                {
                    _frameListener1.reset(nullptr);
                }
                else if (command.code == FrameListenerCommandCode::Create && command.sid == 1ULL && !_frameListener1)
                {
                    _frameListener1.reset(new TestFrameListener1(*_frameListenerManager.get(), _sharedState));
                }

                if (command.code == FrameListenerCommandCode::Delete && command.sid == 2ULL)
                {
                    _frameListener2.reset(nullptr);
                }
                else if (command.code == FrameListenerCommandCode::Create && command.sid == 2ULL && !_frameListener2)
                {
                    _frameListener2.reset(new TestFrameListener2(*_frameListenerManager.get(), _sharedState));
                }

                if (command.code == FrameListenerCommandCode::Delete && command.sid == 3ULL)
                {
                    _frameListener3.reset(nullptr);
                }
                else if (command.code == FrameListenerCommandCode::Create && command.sid == 3ULL && !_frameListener3)
                {
                    _frameListener3.reset(new TestFrameListener3(*_frameListenerManager.get(), _sharedState));
                }

                if (command.code == FrameListenerCommandCode::Delete && command.sid == 4ULL)
                {
                    _frameListener4.reset(nullptr);
                }
                else if (command.code == FrameListenerCommandCode::Create && command.sid == 4ULL && !_frameListener4)
                {
                    _frameListener4.reset(new TestFrameListener4(*_frameListenerManager.get(), _sharedState));
                }
            }
        }

        bool FrameListenersCountOk() const
        {
            return _frameListenersCountOk;
        }

        // public for simplicity
    public:
        UPtr<TestMainFrameListener> mainFrameListener;

    private:
        Window& _mainWindow;
        SharedState _sharedState;
        UPtr<TestFrameListener1> _frameListener1;
        UPtr<TestFrameListener2> _frameListener2;
        UPtr<TestFrameListener3> _frameListener3;
        UPtr<TestFrameListener4> _frameListener4;
        bool _frameListenersCountOk;
    };
}
//--------------------------------------------------------------------------


TEST_CASE("Test window application", "[window_application][application][window][event]")
{
    auto application = Kmplete::CreateUPtr<Kmplete::TestWindowApplication>(Kmplete::WindowApplicationParameters{ .applicationParameters{"TestApplication", "", KMP_TEST_SETTINGS_JSON}, .resizable = true });

    REQUIRE(application);
    REQUIRE(!Kmplete::Filesystem::GetCurrentFilepath().empty());

    REQUIRE(application->FrameListenersCountOk());
    REQUIRE_FALSE(application->mainFrameListener->IsKeyPressEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsKeyReleaseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsKeyCharEventInvoked());

    REQUIRE_FALSE(application->mainFrameListener->IsMouseMoveEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsMouseScrollEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsMouseButtonPressEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsMouseButtonReleaseEventInvoked());

    REQUIRE_FALSE(application->mainFrameListener->IsWindowCloseEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowMoveEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowResizeEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowFocusEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowIconifyEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowFramebufferRefreshEventInvoked());
    REQUIRE_FALSE(application->mainFrameListener->IsWindowFramebufferResizeEventInvoked());

    REQUIRE_FALSE(application->mainFrameListener->IsCustomEventInvoked());

    REQUIRE(application->mainFrameListener->DefaultSizeIsNotZero());
    REQUIRE(application->mainFrameListener->DefaultWindowedSizeIsNotZero());
    REQUIRE(application->mainFrameListener->DuplicateSidFrameListenerCheck());
    REQUIRE(application->mainFrameListener->DuplicatePriorityFrameListenerCheck());

    application->Run();

    REQUIRE(application->mainFrameListener->IsKeyPressEventInvoked());
    REQUIRE(application->mainFrameListener->IsKeyReleaseEventInvoked());
    REQUIRE(application->mainFrameListener->IsKeyCharEventInvoked());

    REQUIRE(application->mainFrameListener->IsMouseMoveEventInvoked());
    REQUIRE(application->mainFrameListener->IsMouseScrollEventInvoked());
    REQUIRE(application->mainFrameListener->IsMouseButtonPressEventInvoked());
    REQUIRE(application->mainFrameListener->IsMouseButtonReleaseEventInvoked());

    REQUIRE(application->mainFrameListener->IsWindowCloseEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowMoveEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowResizeEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowFocusEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowIconifyEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowFramebufferRefreshEventInvoked());
    REQUIRE(application->mainFrameListener->IsWindowFramebufferResizeEventInvoked());

    REQUIRE(application->mainFrameListener->IsCustomEventInvoked());

    REQUIRE(application->mainFrameListener->MousePositionIsNotZero());
    REQUIRE(application->mainFrameListener->DPIIsNotZero());
    REQUIRE(application->mainFrameListener->DPIScaleIsNotZero());

    application.reset();

    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append(KMP_TEST_SETTINGS_JSON);
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(settingsFilepath));
    REQUIRE(Kmplete::Filesystem::FilepathExists(settingsFilepath));
}
//--------------------------------------------------------------------------
