#pragma once

#include "shared_state.h"

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Input/key_codes.h"


namespace Kmplete
{
    class TestFrameListener1Delegate
    {
    public:
        explicit TestFrameListener1Delegate(Events::EventDispatcher& eventDispatcher)
            : str("D")
            , mbcHandlerGuard(eventDispatcher, KMP_BIND(TestFrameListener1Delegate::OnMouseButtonPressed))
        {}

        bool OnMouseButtonPressed(Events::MouseButtonPressEvent&)
        {
            str += "D";
            return true;
        }

        String GetValue() const
        {
            return str;
        }

        String str;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> mbcHandlerGuard;
    };
    //--------------------------------------------------------------------------

    class TestFrameListener1 : public FrameListener
    {
    public:
        TestFrameListener1(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 1ULL, 1)
            , sharedState(sharedState)
            , delegate(nullptr)
            , mbcHandlerGuard(_eventDispatcher, KMP_BIND(TestFrameListener1::OnMouseButtonPressed))
        {
            sharedState.existenceMask |= frame1Mask;

            delegate = CreateUPtr<TestFrameListener1Delegate>(_eventDispatcher);
        }

        ~TestFrameListener1()
        {
            sharedState.existenceMask &= ~frame1Mask;
        }

        void Update(float, bool) override
        {
            sharedState.updateMask |= frame1Mask;
            sharedState.updateMaskString += "1";
        }

        bool OnMouseButtonPressed(Events::MouseButtonPressEvent& event)
        {
            sharedState.eventProcessingString += "1_";
            if (delegate)
            {
                sharedState.eventProcessingString += delegate->GetValue();
            }

            if (event.GetMods() & Input::Mode::Alt)
            {
                if (delegate)
                {
                    delegate.reset(nullptr);
                }
            }
            else
            {
                if (delegate == nullptr)
                {
                    delegate.reset(new TestFrameListener1Delegate(_eventDispatcher));
                }
            }

            if (sharedState.eventAcceptMask & frame1Mask)
            {
                event.handled = true;
            }

            return true;
        }

        SharedState& sharedState;
        const String name = "TestFrameListener1";
        UPtr<TestFrameListener1Delegate> delegate;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> mbcHandlerGuard;
    };
    //--------------------------------------------------------------------------

    class TestFrameListener2 : public FrameListener
    {
    public:
        TestFrameListener2(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 2ULL, 2)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame2Mask;
        }

        ~TestFrameListener2()
        {
            sharedState.existenceMask &= ~frame2Mask;
        }

        void Update(float, bool) override
        {
            sharedState.updateMask |= frame2Mask;
            sharedState.updateMaskString += "2";
        }

        void OnEvent(Events::Event& event) override
        {
            sharedState.eventProcessingString += "2";

            if (sharedState.eventAcceptMask & frame2Mask)
            {
                event.handled = true;
            }
        }

        SharedState& sharedState;
        const String name = "TestFrameListener2";
    };
    //--------------------------------------------------------------------------

    class TestFrameListener3 : public FrameListener
    {
    public:
        TestFrameListener3(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 3ULL, 3)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame3Mask;
        }

        ~TestFrameListener3()
        {
            sharedState.existenceMask &= ~frame3Mask;
        }

        void Update(float, bool) override
        {
            sharedState.updateMask |= frame3Mask;
            sharedState.updateMaskString += "3";
        }

        void OnEvent(Events::Event& event) override
        {
            sharedState.eventProcessingString += "3";

            if (sharedState.eventAcceptMask & frame3Mask)
            {
                event.handled = true;
            }
        }

        SharedState& sharedState;
        const String name = "TestFrameListener3";
    };
    //--------------------------------------------------------------------------

    class TestFrameListener4 : public FrameListener
    {
    public:
        TestFrameListener4(FrameListenerManager& frameListenerManager, SharedState& sharedState)
            : FrameListener(frameListenerManager, 4ULL, 4)
            , sharedState(sharedState)
        {
            sharedState.existenceMask |= frame4Mask;
        }

        ~TestFrameListener4()
        {
            sharedState.existenceMask &= ~frame4Mask;
        }

        void Update(float, bool) override
        {
            sharedState.updateMask |= frame4Mask;
            sharedState.updateMaskString += "4";
        }

        void OnEvent(Events::Event& event) override
        {
            sharedState.eventProcessingString += "4";

            if (sharedState.eventAcceptMask & frame4Mask)
            {
                event.handled = true;
            }
        }

        SharedState& sharedState;
        const String name = "TestFrameListener4";
    };
    //--------------------------------------------------------------------------

    class TestDuplicateSidFrameListener : public FrameListener
    {
    public:
        TestDuplicateSidFrameListener(FrameListenerManager& frameListenerManager)
            : FrameListener(frameListenerManager, 10ULL, 100) // same sid with main frame listener
        {}
    };
    //--------------------------------------------------------------------------

    class TestDuplicatePriorityFrameListener : public FrameListener
    {
    public:
        TestDuplicatePriorityFrameListener(FrameListenerManager& frameListenerManager)
            : FrameListener(frameListenerManager, 100ULL, 10) // same priority with main frame listener
        {}
    };
    //--------------------------------------------------------------------------
}