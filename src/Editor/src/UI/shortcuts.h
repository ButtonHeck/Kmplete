#pragma once

#include "Kmplete/Input/input_codes.h"


namespace Kmplete
{
    namespace Shortcuts
    {
        struct Shortcut
        {
            const char* text;
            Input::KeyModifier mode;
            Input::InputCode key;

            inline bool Accept(Input::KeyModifier modeArg, Input::InputCode keyArg) const noexcept
            {
                return (this->mode == modeArg && this->key == keyArg);
            }
        };
        //--------------------------------------------------------------------------

        static constexpr auto Quit =        Shortcut{"Ctrl+Q", Input::Modifier::Ctrl, Input::Code::Key_Q};
        static constexpr auto Fullscreen =  Shortcut{"Alt+Enter", Input::Modifier::Alt, Input::Code::Key_Enter};
        static constexpr auto AlwaysOnTop = Shortcut{"Ctrl+T", Input::Modifier::Ctrl, Input::Code::Key_T};
    }
}