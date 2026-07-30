#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/command_pool.h"
#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Graphics/graphics_chain_unit.h"
#include "Kmplete/Graphics/graphics_chain_handler.h"
#include "Kmplete/Assets/assets_interface.h"


namespace Kmplete
{
    namespace Graphics
    {
        class Texture;
        class Image;


        //! Base class of a graphics API logical device abstraction
        class KMP_API LogicalDevice : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(LogicalDevice)

        public:
            explicit LogicalDevice(GraphicsChainHandler& chainHandler);
            virtual ~LogicalDevice() = default;

            virtual void RecreateResources() {};

            KMP_NODISCARD virtual const Swapchain& GetSwapchain() const noexcept = 0;

            KMP_NODISCARD virtual Nullable<Texture*> CreateTexture(const Image& image, Assets::TextureSubTypeMaskBits subTypeMask) const = 0;
        };
        //--------------------------------------------------------------------------
    }
}