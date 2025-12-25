#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    class Image;


    //! An OpenGL representation of a texture object. Make sure OpenGL has been initialized before
    //! creating objects of this class
    //! @see OpenGLGraphicsBackend
    //! @see Texture
    //! @see Image
    class OpenGLTexture : public Texture
    {
    public:
        KMP_API explicit OpenGLTexture(const Filepath& filepath, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(const Image& image);

    private:
        void _LoadFromImage(const Image& image);
    };
    //--------------------------------------------------------------------------
}