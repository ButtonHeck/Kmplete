#include "Kmplete/Graphics/texture_factory.h"
#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    Nullable<Texture*> TextureFactory::CreateTexture(GraphicsBackendType backendType, const Filepath& filepath, bool flipVertically /*= false*/)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        try
        {
            switch (backendType)
            {
            case GraphicsBackendType::OpenGL:
                return new OpenGLTexture(filepath, flipVertically);

            default:
                break;
            }
        }
        catch (const std::exception&)
        {
            KMP_LOG_ERROR("failed to create texture '{}'", filepath);
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    Nullable<Texture*> TextureFactory::CreateTexture(GraphicsBackendType backendType, const Image& image)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);
        KMP_ASSERT(image.GetPixels());

        switch (backendType)
        {
        case GraphicsBackendType::OpenGL:
            return new OpenGLTexture(image);

        default:
            break;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------
}