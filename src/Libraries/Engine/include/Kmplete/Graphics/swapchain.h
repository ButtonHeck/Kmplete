#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/framebuffer.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class Swapchain
        {
            KMP_DISABLE_COPY_MOVE(Swapchain)

        public:
            KMP_API Swapchain() noexcept;
            virtual ~Swapchain() = default;

        protected:
            Vector<UPtr<Framebuffer>> _framebuffers;
        };
        //--------------------------------------------------------------------------
    }
}