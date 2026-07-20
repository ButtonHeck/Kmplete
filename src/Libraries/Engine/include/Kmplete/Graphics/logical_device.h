#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/command_pool.h"
#include "Kmplete/Graphics/swapchain.h"


namespace Kmplete
{
    namespace Graphics
    {
        class Texture;
        class Image;


        //! Base class of a graphics API logical device abstraction
        class KMP_API LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(LogicalDevice)

        public:
            LogicalDevice() = default;
            virtual ~LogicalDevice() = default;

            virtual void StartFrame(float /*frameTimestep*/) {};
            virtual void EndFrame() {};
            virtual void HandleWindowResize() {};

            KMP_NODISCARD virtual const Swapchain& GetSwapchain() const noexcept = 0;

            KMP_NODISCARD virtual Nullable<Texture*> CreateTexture(const Image& image) const = 0;
        };
        //--------------------------------------------------------------------------
    }
}