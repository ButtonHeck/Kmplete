#pragma once


namespace Kmplete
{
    namespace Input
    {
        using InputCode = int;
        using KeyModifier = int;


        namespace Code
        {
            enum : InputCode // from glfw3.h
            {
                Mouse_Button0 = 0,
                Mouse_Button1 = 1,
                Mouse_Button2 = 2,
                Mouse_Button3 = 3,
                Mouse_Button4 = 4,
                Mouse_Button5 = 5,
                Mouse_Button6 = 6,
                Mouse_Button7 = 7,

                Mouse_ButtonLast = Mouse_Button7,
                Mouse_ButtonLeft = Mouse_Button0,
                Mouse_ButtonRight = Mouse_Button1,
                Mouse_ButtonMiddle = Mouse_Button2,

                Mouse_Position = 8,
                Mouse_Move = 9,

                Key_Space = 32,
                Key_Apostrophe = 39, // '
                Key_Comma = 44,      // ,
                Key_Minus = 45,      // -
                Key_Period = 46,     // .
                Key_Slash = 47,      // /

                Key_0 = 48, // 0
                Key_1 = 49, // 1
                Key_2 = 50, // 2
                Key_3 = 51, // 3
                Key_4 = 52, // 4
                Key_5 = 53, // 5
                Key_6 = 54, // 6
                Key_7 = 55, // 7
                Key_8 = 56, // 8
                Key_9 = 57, // 9

                Key_Semicolon = 59, // ;
                Key_Equal = 61,     // =

                Key_A = 65,
                Key_B = 66,
                Key_C = 67,
                Key_D = 68,
                Key_E = 69,
                Key_F = 70,
                Key_G = 71,
                Key_H = 72,
                Key_I = 73,
                Key_J = 74,
                Key_K = 75,
                Key_L = 76,
                Key_M = 77,
                Key_N = 78,
                Key_O = 79,
                Key_P = 80,
                Key_Q = 81,
                Key_R = 82,
                Key_S = 83,
                Key_T = 84,
                Key_U = 85,
                Key_V = 86,
                Key_W = 87,
                Key_X = 88,
                Key_Y = 89,
                Key_Z = 90,

                Key_LeftBracket = 91,   // [
                Key_Backslash = 92,     /* \ */
                Key_RightBracket = 93,  // ]
                Key_GraveAccent = 96,   // `

                Key_World1 = 161, // non-US #1
                Key_World2 = 162, // non-US #2

                Key_Escape = 256,
                Key_Enter = 257,
                Key_Tab = 258,
                Key_Backspace = 259,
                Key_Insert = 260,
                Key_Delete = 261,
                Key_Right = 262,
                Key_Left = 263,
                Key_Down = 264,
                Key_Up = 265,
                Key_PageUp = 266,
                Key_PageDown = 267,
                Key_Home = 268,
                Key_End = 269,
                Key_CapsLock = 280,
                Key_ScrollLock = 281,
                Key_NumLock = 282,
                Key_PrintScreen = 283,
                Key_Pause = 284,
                Key_F1 = 290,
                Key_F2 = 291,
                Key_F3 = 292,
                Key_F4 = 293,
                Key_F5 = 294,
                Key_F6 = 295,
                Key_F7 = 296,
                Key_F8 = 297,
                Key_F9 = 298,
                Key_F10 = 299,
                Key_F11 = 300,
                Key_F12 = 301,
                Key_F13 = 302,
                Key_F14 = 303,
                Key_F15 = 304,
                Key_F16 = 305,
                Key_F17 = 306,
                Key_F18 = 307,
                Key_F19 = 308,
                Key_F20 = 309,
                Key_F21 = 310,
                Key_F22 = 311,
                Key_F23 = 312,
                Key_F24 = 313,
                Key_F25 = 314,

                Key_KP0 = 320,
                Key_KP1 = 321,
                Key_KP2 = 322,
                Key_KP3 = 323,
                Key_KP4 = 324,
                Key_KP5 = 325,
                Key_KP6 = 326,
                Key_KP7 = 327,
                Key_KP8 = 328,
                Key_KP9 = 329,
                Key_KPDecimal = 330,
                Key_KPDivide = 331,
                Key_KPMultiply = 332,
                Key_KPSubtract = 333,
                Key_KPAdd = 334,
                Key_KPEnter = 335,
                Key_KPEqual = 336,

                Key_LeftShift = 340,
                Key_LeftControl = 341,
                Key_LeftAlt = 342,
                Key_LeftSuper = 343,
                Key_RightShift = 344,
                Key_RightControl = 345,
                Key_RightAlt = 346,
                Key_RightSuper = 347,
                Key_Menu = 348,

                NumCodes
            };
        }
        //--------------------------------------------------------------------------

        constexpr const char* GetCodeName(InputCode code)
        {
            switch (code)
            {
                case Code::Mouse_Button0:   return "LMB";
                case Code::Mouse_Button1:   return "RMB";
                case Code::Mouse_Button2:   return "Wheel";
                case Code::Mouse_Button3:   return "Mouse 3";
                case Code::Mouse_Button4:   return "Mouse 4";
                case Code::Mouse_Button5:   return "Mouse 5";
                case Code::Mouse_Button6:   return "Mouse 6";
                case Code::Mouse_Button7:   return "Mouse 7";
                case Code::Mouse_Position:  return "Mouse position";
                case Code::Mouse_Move:      return "Mouse move";

                case Code::Key_Space:        return "Space";
                case Code::Key_Apostrophe:   return "'";
                case Code::Key_Comma:        return ",";
                case Code::Key_Minus:        return "-";
                case Code::Key_Period:       return ".";
                case Code::Key_Slash:        return "/";

                case Code::Key_0: return "0";
                case Code::Key_1: return "1";
                case Code::Key_2: return "2";
                case Code::Key_3: return "3";
                case Code::Key_4: return "4";
                case Code::Key_5: return "5";
                case Code::Key_6: return "6";
                case Code::Key_7: return "7";
                case Code::Key_8: return "8";
                case Code::Key_9: return "9";

                case Code::Key_Semicolon:    return ";";
                case Code::Key_Equal:        return "=";

                case Code::Key_A: return "A";
                case Code::Key_B: return "B";
                case Code::Key_C: return "C";
                case Code::Key_D: return "D";
                case Code::Key_E: return "E";
                case Code::Key_F: return "F";
                case Code::Key_G: return "G";
                case Code::Key_H: return "H";
                case Code::Key_I: return "I";
                case Code::Key_J: return "J";
                case Code::Key_K: return "K";
                case Code::Key_L: return "L";
                case Code::Key_M: return "M";
                case Code::Key_N: return "N";
                case Code::Key_O: return "O";
                case Code::Key_P: return "P";
                case Code::Key_Q: return "Q";
                case Code::Key_R: return "R";
                case Code::Key_S: return "S";
                case Code::Key_T: return "T";
                case Code::Key_U: return "U";
                case Code::Key_V: return "V";
                case Code::Key_W: return "W";
                case Code::Key_X: return "X";
                case Code::Key_Y: return "Y";
                case Code::Key_Z: return "Z";

                case Code::Key_LeftBracket:  return "[";
                case Code::Key_Backslash:    return "\\";
                case Code::Key_RightBracket: return "]";
                case Code::Key_GraveAccent:  return "`";

                case Code::Key_World1: return "World1";
                case Code::Key_World2: return "World2";

                case Code::Key_Escape:       return "Esc";
                case Code::Key_Enter:        return "Enter";
                case Code::Key_Tab:          return "Tab";
                case Code::Key_Backspace:    return "Backspace";
                case Code::Key_Insert:       return "Ins";
                case Code::Key_Delete:       return "Del";
                case Code::Key_Right:        return "Right";
                case Code::Key_Left:         return "Left";
                case Code::Key_Down:         return "Down";
                case Code::Key_Up:           return "Up";
                case Code::Key_PageUp:       return "Page Up";
                case Code::Key_PageDown:     return "Page Down";
                case Code::Key_Home:         return "Home";
                case Code::Key_End:          return "End";
                case Code::Key_CapsLock:     return "Caps Lock";
                case Code::Key_ScrollLock:   return "Scroll Lock";
                case Code::Key_NumLock:      return "Num Lock";
                case Code::Key_PrintScreen:  return "Printscreen";
                case Code::Key_Pause:        return "Pause";
                case Code::Key_F1:           return "F1";
                case Code::Key_F2:           return "F2";
                case Code::Key_F3:           return "F3";
                case Code::Key_F4:           return "F4";
                case Code::Key_F5:           return "F5";
                case Code::Key_F6:           return "F6";
                case Code::Key_F7:           return "F7";
                case Code::Key_F8:           return "F8";
                case Code::Key_F9:           return "F9";
                case Code::Key_F10:          return "F10";
                case Code::Key_F11:          return "F11";
                case Code::Key_F12:          return "F12";
                case Code::Key_F13:          return "F13";
                case Code::Key_F14:          return "F14";
                case Code::Key_F15:          return "F15";
                case Code::Key_F16:          return "F16";
                case Code::Key_F17:          return "F17";
                case Code::Key_F18:          return "F18";
                case Code::Key_F19:          return "F19";
                case Code::Key_F20:          return "F20";
                case Code::Key_F21:          return "F21";
                case Code::Key_F22:          return "F22";
                case Code::Key_F23:          return "F23";
                case Code::Key_F24:          return "F24";
                case Code::Key_F25:          return "F25";

                case Code::Key_KP0:          return "Num 0";
                case Code::Key_KP1:          return "Num 1";
                case Code::Key_KP2:          return "Num 2";
                case Code::Key_KP3:          return "Num 3";
                case Code::Key_KP4:          return "Num 4";
                case Code::Key_KP5:          return "Num 5";
                case Code::Key_KP6:          return "Num 6";
                case Code::Key_KP7:          return "Num 7";
                case Code::Key_KP8:          return "Num 8";
                case Code::Key_KP9:          return "Num 9";
                case Code::Key_KPDecimal:    return "Num .";
                case Code::Key_KPDivide:     return "Num /";
                case Code::Key_KPMultiply:   return "Num *";
                case Code::Key_KPSubtract:   return "Num -";
                case Code::Key_KPAdd:        return "Num +";
                case Code::Key_KPEnter:      return "Num Enter";
                case Code::Key_KPEqual:      return "Num =";

                case Code::Key_LeftShift:    return "LShift";
                case Code::Key_LeftControl:  return "LCtrl";
                case Code::Key_LeftAlt:      return "LAlt";
                case Code::Key_LeftSuper:    return "LSuper";
                case Code::Key_RightShift:   return "RShift";
                case Code::Key_RightControl: return "RCtrl";
                case Code::Key_RightAlt:     return "RAlt";
                case Code::Key_RightSuper:   return "RSuper";
                case Code::Key_Menu:         return "Menu";

                default: break;
            }

            return "";
        }
        //--------------------------------------------------------------------------


        namespace Modifier
        {
            enum : KeyModifier
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

        constexpr const char* GetKeyModifierName(KeyModifier mode)
        {
            switch (mode)
            {
                case Modifier::None:        return "";
                case Modifier::Shift:       return "Shift";
                case Modifier::Ctrl:        return "Ctrl";
                case Modifier::Alt:         return "Alt";
                case Modifier::Super:       return "Super";
                case Modifier::CapsLock:    return "CapsLock";
                case Modifier::NumLock:     return "NumLock";

                default: break;
            }

            return "";
        }
        //--------------------------------------------------------------------------
    }
}