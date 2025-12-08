#pragma once


namespace Kmplete
{
    namespace Input
    {
        using MouseCode = int;


        namespace Mouse
        {
            enum : MouseCode // from glfw3.h
            {
                Button0 = 0,
                Button1 = 1,
                Button2 = 2,
                Button3 = 3,
                Button4 = 4,
                Button5 = 5,
                Button6 = 6,
                Button7 = 7,
                NumButtons,

                ButtonLast = Button7,
                ButtonLeft = Button0,
                ButtonRight = Button1,
                ButtonMiddle = Button2
            };
        }
        //--------------------------------------------------------------------------

        constexpr const char* GetMouseCodeName(MouseCode code)
        {
            switch (code)
            {
                case Mouse::Button0: return "LMB";
                case Mouse::Button1: return "RMB";
                case Mouse::Button2: return "Wheel";
                case Mouse::Button3: return "Mouse 3";
                case Mouse::Button4: return "Mouse 4";
                case Mouse::Button5: return "Mouse 5";
                case Mouse::Button6: return "Mouse 6";
                case Mouse::Button7: return "Mouse 7";

                default: break;
            }

            return "";
        }
        //--------------------------------------------------------------------------
    }
}
