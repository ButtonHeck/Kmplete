#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Math/geometry.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Properties description of a single character in a font
        struct FontCharacter
        {
            Math::Vec2F uvMin;
            Math::Vec2F uvMax;
            Math::Vec2I size;
            Math::Vec2I bearing;
            UInt32 advance;
        };
        //--------------------------------------------------------------------------


        //! Description of a single character's vertex for rendering
        struct FontCharacterVertex
        {
            Math::Vec2F position;
            Math::Vec2F uv;
        };
        //--------------------------------------------------------------------------
    }
}