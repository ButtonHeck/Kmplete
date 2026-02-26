#include "Kmplete/Graphics/texture_factory.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        Nullable<Texture*> TextureFactory::CreateTexture(GraphicsBackendType backendType, const Filepath& filepath, KMP_MB_UNUSED bool flipVertically /*= false*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            try
            {
                switch (backendType)
                {
                case GraphicsBackendType::Vulkan:
                    KMP_LOG_WARN("not implemented yet"); (void)filepath;
                    break;

                default:
                    KMP_LOG_ERROR("unknown graphics backend type");
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
            case GraphicsBackendType::Vulkan:
                KMP_LOG_WARN("not implemented yet"); (void)image;
                break;

            default:
                KMP_LOG_ERROR("unknown graphics backend type");
                break;
            }

            return nullptr;
        }
        //--------------------------------------------------------------------------
    }
}