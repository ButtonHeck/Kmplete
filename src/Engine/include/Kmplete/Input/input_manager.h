#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Input/mouse_codes.h"
#include "Kmplete/Input/key_codes.h"
#include "Kmplete/Math/geometry.h"


namespace Kmplete
{
    namespace Events
    {
        struct Event;
    }


    namespace Input
    {
        class InputManager
        {
            KMP_DISABLE_COPY_MOVE(InputManager)
            KMP_LOG_CLASSNAME(InputManager)

        public:
            KMP_API InputManager() noexcept;

            KMP_API void OnEvent(Events::Event& event);

            KMP_NODISCARD KMP_API const Math::Point2I& GetMousePosition() const noexcept;
            KMP_NODISCARD KMP_API bool IsMouseButtonPressed(MouseCode mouseCode) const;

        private:
            Math::Point2I _mousePosition;
            Array<bool, Mouse::NumButtons> _mouseButtonsStates;
        };
        //--------------------------------------------------------------------------
    }
}