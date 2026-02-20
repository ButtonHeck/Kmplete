#pragma once

#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_surface.h"
#include "Kmplete/Graphics/OpenGL/opengl_physical_device.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //! OpenGL backend implementation, loading is delegated to GLAD init function.
        //! Indirectly depends on the window backend, thus a window backend should be 
        //! initialized before this
        //! @see GraphicsBackend
        class OpenGLGraphicsBackend : public GraphicsBackend
        {
            KMP_LOG_CLASSNAME(OpenGLGraphicsBackend)
            KMP_DISABLE_COPY_MOVE(OpenGLGraphicsBackend)

        public:
            KMP_API explicit OpenGLGraphicsBackend(Window& window);
            KMP_API ~OpenGLGraphicsBackend();

            KMP_NODISCARD KMP_API const GraphicsSurface& GetGraphicsSurface() const noexcept override;
            KMP_NODISCARD KMP_API const PhysicalDevice& GetPhysicalDevice() const noexcept override;

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;

        private:
            void _Initialize();
            void _Finalize();

        private:
            UPtr<OpenGLGraphicsSurface> _surface;
            UPtr<OpenGLPhysicalDevice> _physicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}