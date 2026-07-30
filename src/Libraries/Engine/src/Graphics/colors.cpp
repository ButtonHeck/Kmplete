#include "Kmplete/Graphics/colors.h"

#include <glm/glm.hpp>
#include <glm/gtc/color_space.hpp>


namespace Kmplete
{
    namespace Graphics
    {
        namespace Colors
        {
            Color ToSRBG(const Color& rgbaLinear)
            {
                const auto rgb = Math::Vec3F(rgbaLinear);
                return Color(glm::convertLinearToSRGB(rgb), rgbaLinear.a);
            }
            //--------------------------------------------------------------------------

            Color ToLinear(const Color& rgbaSRGB)
            {
                const auto rgb = Math::Vec3F(rgbaSRGB);
                return Color(glm::convertSRGBToLinear(rgb), rgbaSRGB.a);
            }
            //--------------------------------------------------------------------------
        }
    }
}
