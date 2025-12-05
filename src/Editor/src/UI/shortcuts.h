#pragma once

#include "Kmplete/Input/key_codes.h"


namespace Kmplete
{
    namespace Shortcuts
    {
        struct Shortcut
        {
            const char* text;
            Input::KeyMode mode;
            Input::KeyCode key;

            inline bool Accept(Input::KeyMode modeArg, Input::KeyCode keyArg) const noexcept
            {
                return (this->mode == modeArg && this->key == keyArg);
            }
        };
        //--------------------------------------------------------------------------

        static constexpr auto Quit =        Shortcut{"Ctrl+Q", Input::Mode::Ctrl, Input::Key::Q};
        static constexpr auto Fullscreen =  Shortcut{"Alt+Enter", Input::Mode::Alt, Input::Key::Enter};
        static constexpr auto AlwaysOnTop = Shortcut{"Ctrl+T", Input::Mode::Ctrl, Input::Key::T};
    }
}