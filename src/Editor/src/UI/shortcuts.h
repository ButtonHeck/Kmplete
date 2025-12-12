#pragma once

#include "Kmplete/Input/key_codes.h"


namespace Kmplete
{
    namespace Shortcuts
    {
        struct Shortcut
        {
            const char* text;
            Input::KeyModifier mode;
            Input::KeyCode key;

            inline bool Accept(Input::KeyModifier modeArg, Input::KeyCode keyArg) const noexcept
            {
                return (this->mode == modeArg && this->key == keyArg);
            }
        };
        //--------------------------------------------------------------------------

        static constexpr auto Quit =        Shortcut{"Ctrl+Q", Input::Modifier::Ctrl, Input::Key::Q};
        static constexpr auto Fullscreen =  Shortcut{"Alt+Enter", Input::Modifier::Alt, Input::Key::Enter};
        static constexpr auto AlwaysOnTop = Shortcut{"Ctrl+T", Input::Modifier::Ctrl, Input::Key::T};
    }
}