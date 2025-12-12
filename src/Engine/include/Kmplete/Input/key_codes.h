#pragma once


namespace Kmplete
{
    namespace Input
    {
        using KeyCode = int;
        using KeyMode = int;


        namespace Mode
        {
            enum : KeyMode
            {
                None = 0,
                Shift = 0x0001,
                Ctrl = 0x0002,
                Alt = 0x0004,
                Super = 0x0008,
                CapsLock = 0x0010,
                NumLock = 0x0020
            };
        }
        //--------------------------------------------------------------------------

        constexpr const char* GetKeyModeName(KeyMode mode)
        {
            switch (mode)
            {
                case Mode::None:        return "";
                case Mode::Shift:       return "Shift";
                case Mode::Ctrl:        return "Ctrl";
                case Mode::Alt:         return "Alt";
                case Mode::Super:       return "Super";
                case Mode::CapsLock:    return "CapsLock";
                case Mode::NumLock:     return "NumLock";

                default: break;
            }

            return "";
        }
        //--------------------------------------------------------------------------


        namespace Key
        {
            enum : KeyCode // from glfw3.h
            {
                First = 32,
                Space = First,
                Apostrophe = 39, // '
                Comma = 44,      // ,
                Minus = 45,      // -
                Period = 46,     // .
                Slash = 47,      // /

                D0 = 48, // 0
                D1 = 49, // 1
                D2 = 50, // 2
                D3 = 51, // 3
                D4 = 52, // 4
                D5 = 53, // 5
                D6 = 54, // 6
                D7 = 55, // 7
                D8 = 56, // 8
                D9 = 57, // 9

                Semicolon = 59, // ;
                Equal = 61,     // =

                A = 65,
                B = 66,
                C = 67,
                D = 68,
                E = 69,
                F = 70,
                G = 71,
                H = 72,
                I = 73,
                J = 74,
                K = 75,
                L = 76,
                M = 77,
                N = 78,
                O = 79,
                P = 80,
                Q = 81,
                R = 82,
                S = 83,
                T = 84,
                U = 85,
                V = 86,
                W = 87,
                X = 88,
                Y = 89,
                Z = 90,

                LeftBracket = 91,   // [
                Backslash = 92,     /* \ */
                RightBracket = 93,  // ]
                GraveAccent = 96,   // `

                World1 = 161, // non-US #1
                World2 = 162, // non-US #2

                Escape = 256,
                Enter = 257,
                Tab = 258,
                Backspace = 259,
                Insert = 260,
                Delete = 261,
                Right = 262,
                Left = 263,
                Down = 264,
                Up = 265,
                PageUp = 266,
                PageDown = 267,
                Home = 268,
                End = 269,
                CapsLock = 280,
                ScrollLock = 281,
                NumLock = 282,
                PrintScreen = 283,
                Pause = 284,
                F1 = 290,
                F2 = 291,
                F3 = 292,
                F4 = 293,
                F5 = 294,
                F6 = 295,
                F7 = 296,
                F8 = 297,
                F9 = 298,
                F10 = 299,
                F11 = 300,
                F12 = 301,
                F13 = 302,
                F14 = 303,
                F15 = 304,
                F16 = 305,
                F17 = 306,
                F18 = 307,
                F19 = 308,
                F20 = 309,
                F21 = 310,
                F22 = 311,
                F23 = 312,
                F24 = 313,
                F25 = 314,

                KP0 = 320,
                KP1 = 321,
                KP2 = 322,
                KP3 = 323,
                KP4 = 324,
                KP5 = 325,
                KP6 = 326,
                KP7 = 327,
                KP8 = 328,
                KP9 = 329,
                KPDecimal = 330,
                KPDivide = 331,
                KPMultiply = 332,
                KPSubtract = 333,
                KPAdd = 334,
                KPEnter = 335,
                KPEqual = 336,

                LeftShift = 340,
                LeftControl = 341,
                LeftAlt = 342,
                LeftSuper = 343,
                RightShift = 344,
                RightControl = 345,
                RightAlt = 346,
                RightSuper = 347,
                Menu = 348,

                NumKeys
            };
        }
        //--------------------------------------------------------------------------

        constexpr const char* GetKeyCodeName(KeyCode code)
        {
            switch (code)
            {
                case Key::Space:        return "Space";
                case Key::Apostrophe:   return "'";
                case Key::Comma:        return ",";
                case Key::Minus:        return "-";
                case Key::Period:       return ".";
                case Key::Slash:        return "/";

                case Key::D0: return "0";
                case Key::D1: return "1";
                case Key::D2: return "2";
                case Key::D3: return "3";
                case Key::D4: return "4";
                case Key::D5: return "5";
                case Key::D6: return "6";
                case Key::D7: return "7";
                case Key::D8: return "8";
                case Key::D9: return "9";

                case Key::Semicolon:    return ";";
                case Key::Equal:        return "=";

                case Key::A: return "A";
                case Key::B: return "B";
                case Key::C: return "C";
                case Key::D: return "D";
                case Key::E: return "E";
                case Key::F: return "F";
                case Key::G: return "G";
                case Key::H: return "H";
                case Key::I: return "I";
                case Key::J: return "J";
                case Key::K: return "K";
                case Key::L: return "L";
                case Key::M: return "M";
                case Key::N: return "N";
                case Key::O: return "O";
                case Key::P: return "P";
                case Key::Q: return "Q";
                case Key::R: return "R";
                case Key::S: return "S";
                case Key::T: return "T";
                case Key::U: return "U";
                case Key::V: return "V";
                case Key::W: return "W";
                case Key::X: return "X";
                case Key::Y: return "Y";
                case Key::Z: return "Z";

                case Key::LeftBracket:  return "[";
                case Key::Backslash:    return "\\";
                case Key::RightBracket: return "]";
                case Key::GraveAccent:  return "`";

                case Key::World1: return "World1";
                case Key::World2: return "World2";

                case Key::Escape:       return "Esc";
                case Key::Enter:        return "Enter";
                case Key::Tab:          return "Tab";
                case Key::Backspace:    return "Backspace";
                case Key::Insert:       return "Ins";
                case Key::Delete:       return "Del";
                case Key::Right:        return "Right";
                case Key::Left:         return "Left";
                case Key::Down:         return "Down";
                case Key::Up:           return "Up";
                case Key::PageUp:       return "Page Up";
                case Key::PageDown:     return "Page Down";
                case Key::Home:         return "Home";
                case Key::End:          return "End";
                case Key::CapsLock:     return "Caps Lock";
                case Key::ScrollLock:   return "Scroll Lock";
                case Key::NumLock:      return "Num Lock";
                case Key::PrintScreen:  return "Printscreen";
                case Key::Pause:        return "Pause";
                case Key::F1:           return "F1";
                case Key::F2:           return "F2";
                case Key::F3:           return "F3";
                case Key::F4:           return "F4";
                case Key::F5:           return "F5";
                case Key::F6:           return "F6";
                case Key::F7:           return "F7";
                case Key::F8:           return "F8";
                case Key::F9:           return "F9";
                case Key::F10:          return "F10";
                case Key::F11:          return "F11";
                case Key::F12:          return "F12";
                case Key::F13:          return "F13";
                case Key::F14:          return "F14";
                case Key::F15:          return "F15";
                case Key::F16:          return "F16";
                case Key::F17:          return "F17";
                case Key::F18:          return "F18";
                case Key::F19:          return "F19";
                case Key::F20:          return "F20";
                case Key::F21:          return "F21";
                case Key::F22:          return "F22";
                case Key::F23:          return "F23";
                case Key::F24:          return "F24";
                case Key::F25:          return "F25";

                case Key::KP0:          return "Num 0";
                case Key::KP1:          return "Num 1";
                case Key::KP2:          return "Num 2";
                case Key::KP3:          return "Num 3";
                case Key::KP4:          return "Num 4";
                case Key::KP5:          return "Num 5";
                case Key::KP6:          return "Num 6";
                case Key::KP7:          return "Num 7";
                case Key::KP8:          return "Num 8";
                case Key::KP9:          return "Num 9";
                case Key::KPDecimal:    return "Num .";
                case Key::KPDivide:     return "Num /";
                case Key::KPMultiply:   return "Num *";
                case Key::KPSubtract:   return "Num -";
                case Key::KPAdd:        return "Num +";
                case Key::KPEnter:      return "Num Enter";
                case Key::KPEqual:      return "Num =";

                case Key::LeftShift:    return "LShift";
                case Key::LeftControl:  return "LCtrl";
                case Key::LeftAlt:      return "LAlt";
                case Key::LeftSuper:    return "LSuper";
                case Key::RightShift:   return "RShift";
                case Key::RightControl: return "RCtrl";
                case Key::RightAlt:     return "RAlt";
                case Key::RightSuper:   return "RSuper";
                case Key::Menu:         return "Menu";

                default: break;
            }

            return "";
        }
        //--------------------------------------------------------------------------
    }
}
