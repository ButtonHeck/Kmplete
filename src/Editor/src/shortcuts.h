#pragma once

#include "Kmplete/Event/key_codes.h"

namespace Kmplete
{
    namespace Shortcuts
    {
        struct Shortcut
        {
            const char* text;
            KeyMode mode;
            KeyCode key;

            inline bool Accept(KeyMode modeArg, KeyCode keyArg) const
            {
                return (this->mode == modeArg && this->key == keyArg);
            }
        };
        //--------------------------------------------------------------------------

        static constexpr auto Quit = Shortcut{"Ctrl+Q", Mode::Ctrl, Key::Q};
        static constexpr auto Fullscreen = Shortcut{"Alt+Enter", Mode::Alt, Key::Enter};
    }
}