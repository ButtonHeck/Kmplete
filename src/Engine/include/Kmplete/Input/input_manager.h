#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Input/mouse_codes.h"
#include "Kmplete/Input/key_codes.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Events
    {
        struct Event;
    }


    namespace Input
    {
        using InputCode = int;
        using InputControlValue = float;
        using ActionIdentifier = StringID;
        using ActionCallbackTag = StringID;
        using ActionCallback = Function<bool(InputControlValue)>;


        struct TaggedActionCallback
        {
            ActionCallbackTag tag;
            ActionCallback callback;
        };
        //--------------------------------------------------------------------------

        static constexpr ActionCallbackTag DefaultActionCallbackTag = 0ULL;


        struct ActionEvent
        {
            ActionIdentifier id;
            InputControlValue value;
        };
        //--------------------------------------------------------------------------


        class InputManager
        {
            KMP_LOG_CLASSNAME(InputManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_DISABLE_COPY_MOVE(InputManager)

            KMP_API InputManager() noexcept;

            KMP_API void ProcessInputEvents(Events::Event& event);
            KMP_API void PropagateActionEvents();

            KMP_API bool MapActionToCallback(ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapActionToCallback(ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);
            KMP_API bool UnmapActionFromCallback(ActionIdentifier actionId, const ActionCallbackTag& callbackTag);

            KMP_API bool MapInputToAction(InputCode code, ActionIdentifier actionId);
            KMP_API bool UnmapInputFromAction(InputCode code, ActionIdentifier actionId);
            KMP_API bool RemapInputToAction(InputCode code, ActionIdentifier actionId);
            KMP_NODISCARD KMP_API InputControlValue GetActionValue(ActionIdentifier actionId);

            KMP_API bool MapInputToCallback(InputCode code, ActionIdentifier actionId, const ActionCallback& callback);
            KMP_API bool MapInputToCallback(InputCode code, ActionIdentifier actionId, const TaggedActionCallback& taggedCallback);

            KMP_NODISCARD KMP_API const Math::Point2I& GetMousePosition() const noexcept;
            KMP_NODISCARD KMP_API bool IsMouseButtonPressed(MouseCode mouseCode) const;
            KMP_NODISCARD KMP_API KeyModifier GetKeyModifiersMask() const noexcept;
            KMP_NODISCARD KMP_API bool IsKeyButtonPressed(KeyCode keyCode) const;

        private:
            KMP_NODISCARD Vector<ActionEvent> _CreateActionEvents(InputCode code, InputControlValue value) const;
            void _PropagateSingleActionEvent(const ActionEvent& actionEvent);
            KMP_NODISCARD bool _ContainsTaggedCallback(const Vector<TaggedActionCallback>& callbacks, const TaggedActionCallback& taggedCallback) const;
            KMP_NODISCARD bool _ContainsActionIdentifier(const Vector<ActionIdentifier>& actionsIdentifiers, ActionIdentifier actionId) const;

        private:
            Math::Point2I _mousePosition;
            Array<InputControlValue, Mouse::NumButtons> _mouseButtonsStates;
            Array<InputControlValue, Key::NumKeys> _keyButtonsStates;
            KeyModifier _modifiersMask;

            HashMap<InputCode, Vector<ActionIdentifier>> _inputCodeToActionsMap;
            HashMap<ActionIdentifier, Vector<InputCode>> _actionToInputCodesMap;
            HashMap<ActionIdentifier, Vector<TaggedActionCallback>> _actionCallbacks;

            Vector<ActionEvent> _actionEvents;
        };
        //--------------------------------------------------------------------------
    }
}