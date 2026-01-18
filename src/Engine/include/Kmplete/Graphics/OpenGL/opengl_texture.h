#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"


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
        KMP_LOG_CLASSNAME(OpenGLTexture)
        KMP_DISABLE_COPY_MOVE(OpenGLTexture)

    public:
        KMP_API explicit OpenGLTexture(const Filepath& filepath, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(const Image& image);
        KMP_API ~OpenGLTexture();

    private:
        void _LoadFromImage(const Image& image);
    };
    //--------------------------------------------------------------------------
}