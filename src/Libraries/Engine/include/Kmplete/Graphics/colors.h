#pragma once

#include "Kmplete/Math/geometry.h"


namespace Kmplete
{
    namespace Graphics
    {
        namespace Colors
        {
            using Color = Math::Vec4F;

            //                                        R     G     B     A
            static constexpr auto Red =         Color(1.0f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red90 =       Color(0.9f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red80 =       Color(0.8f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red70 =       Color(0.7f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red60 =       Color(0.6f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red50 =       Color(0.5f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red40 =       Color(0.4f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red30 =       Color(0.3f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red20 =       Color(0.2f, 0.0f, 0.0f, 1.0f);
            static constexpr auto Red10 =       Color(0.1f, 0.0f, 0.0f, 1.0f);

            static constexpr auto Blue =        Color(0.0f, 1.0f, 0.0f, 1.0f);
            static constexpr auto Blue90 =      Color(0.0f, 0.9f, 0.0f, 1.0f);
            static constexpr auto Blue80 =      Color(0.0f, 0.8f, 0.0f, 1.0f);
            static constexpr auto Blue70 =      Color(0.0f, 0.7f, 0.0f, 1.0f);
            static constexpr auto Blue60 =      Color(0.0f, 0.6f, 0.0f, 1.0f);
            static constexpr auto Blue50 =      Color(0.0f, 0.5f, 0.0f, 1.0f);
            static constexpr auto Blue40 =      Color(0.0f, 0.4f, 0.0f, 1.0f);
            static constexpr auto Blue30 =      Color(0.0f, 0.3f, 0.0f, 1.0f);
            static constexpr auto Blue20 =      Color(0.0f, 0.2f, 0.0f, 1.0f);
            static constexpr auto Blue10 =      Color(0.0f, 0.1f, 0.0f, 1.0f);

            static constexpr auto Green =       Color(0.0f, 0.0f, 1.0f, 1.0f);
            static constexpr auto Green90 =     Color(0.0f, 0.0f, 0.9f, 1.0f);
            static constexpr auto Green80 =     Color(0.0f, 0.0f, 0.8f, 1.0f);
            static constexpr auto Green70 =     Color(0.0f, 0.0f, 0.7f, 1.0f);
            static constexpr auto Green60 =     Color(0.0f, 0.0f, 0.6f, 1.0f);
            static constexpr auto Green50 =     Color(0.0f, 0.0f, 0.5f, 1.0f);
            static constexpr auto Green40 =     Color(0.0f, 0.0f, 0.4f, 1.0f);
            static constexpr auto Green30 =     Color(0.0f, 0.0f, 0.3f, 1.0f);
            static constexpr auto Green20 =     Color(0.0f, 0.0f, 0.2f, 1.0f);
            static constexpr auto Green10 =     Color(0.0f, 0.0f, 0.1f, 1.0f);

            static constexpr auto Cyan =        Color(0.0f, 1.0f, 1.0f, 1.0f);
            static constexpr auto Cyan90 =      Color(0.0f, 0.9f, 0.9f, 1.0f);
            static constexpr auto Cyan80 =      Color(0.0f, 0.8f, 0.8f, 1.0f);
            static constexpr auto Cyan70 =      Color(0.0f, 0.7f, 0.7f, 1.0f);
            static constexpr auto Cyan60 =      Color(0.0f, 0.6f, 0.6f, 1.0f);
            static constexpr auto Cyan50 =      Color(0.0f, 0.5f, 0.5f, 1.0f);
            static constexpr auto Cyan40 =      Color(0.0f, 0.4f, 0.4f, 1.0f);
            static constexpr auto Cyan30 =      Color(0.0f, 0.3f, 0.3f, 1.0f);
            static constexpr auto Cyan20 =      Color(0.0f, 0.2f, 0.2f, 1.0f);
            static constexpr auto Cyan10 =      Color(0.0f, 0.1f, 0.1f, 1.0f);

            static constexpr auto Magenta =     Color(1.0f, 0.0f, 1.0f, 1.0f);
            static constexpr auto Magenta90 =   Color(0.9f, 0.0f, 0.9f, 1.0f);
            static constexpr auto Magenta80 =   Color(0.8f, 0.0f, 0.8f, 1.0f);
            static constexpr auto Magenta70 =   Color(0.7f, 0.0f, 0.7f, 1.0f);
            static constexpr auto Magenta60 =   Color(0.6f, 0.0f, 0.6f, 1.0f);
            static constexpr auto Magenta50 =   Color(0.5f, 0.0f, 0.5f, 1.0f);
            static constexpr auto Magenta40 =   Color(0.4f, 0.0f, 0.4f, 1.0f);
            static constexpr auto Magenta30 =   Color(0.3f, 0.0f, 0.3f, 1.0f);
            static constexpr auto Magenta20 =   Color(0.2f, 0.0f, 0.2f, 1.0f);
            static constexpr auto Magenta10 =   Color(0.1f, 0.0f, 0.1f, 1.0f);

            static constexpr auto Yellow =      Color(1.0f, 1.0f, 0.0f, 1.0f);
            static constexpr auto Yellow90 =    Color(0.9f, 0.9f, 0.0f, 1.0f);
            static constexpr auto Yellow80 =    Color(0.8f, 0.8f, 0.0f, 1.0f);
            static constexpr auto Yellow70 =    Color(0.7f, 0.7f, 0.0f, 1.0f);
            static constexpr auto Yellow60 =    Color(0.6f, 0.6f, 0.0f, 1.0f);
            static constexpr auto Yellow50 =    Color(0.5f, 0.5f, 0.0f, 1.0f);
            static constexpr auto Yellow40 =    Color(0.4f, 0.4f, 0.0f, 1.0f);
            static constexpr auto Yellow30 =    Color(0.3f, 0.3f, 0.0f, 1.0f);
            static constexpr auto Yellow20 =    Color(0.2f, 0.2f, 0.0f, 1.0f);
            static constexpr auto Yellow10 =    Color(0.1f, 0.1f, 0.0f, 1.0f);

            static constexpr auto White =       Color(1.0f, 1.0f, 1.0f, 1.0f);
            static constexpr auto Grey90 =      Color(0.9f, 0.9f, 0.9f, 1.0f);
            static constexpr auto Grey80 =      Color(0.8f, 0.8f, 0.8f, 1.0f);
            static constexpr auto Grey70 =      Color(0.7f, 0.7f, 0.7f, 1.0f);
            static constexpr auto Grey60 =      Color(0.6f, 0.6f, 0.6f, 1.0f);
            static constexpr auto Grey50 =      Color(0.5f, 0.5f, 0.5f, 1.0f);
            static constexpr auto Grey40 =      Color(0.4f, 0.4f, 0.4f, 1.0f);
            static constexpr auto Grey30 =      Color(0.3f, 0.3f, 0.3f, 1.0f);
            static constexpr auto Grey20 =      Color(0.2f, 0.2f, 0.2f, 1.0f);
            static constexpr auto Grey10 =      Color(0.1f, 0.1f, 0.1f, 1.0f);
            static constexpr auto Black =       Color(0.0f, 0.0f, 0.0f, 1.0f);


            KMP_NODISCARD KMP_API Color ToSRBG(const Color& rgbaLinear);
            KMP_NODISCARD KMP_API Color ToLinear(const Color& rgbaSRGB);
        }
    }
}