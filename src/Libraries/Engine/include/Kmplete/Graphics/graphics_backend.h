#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/graphics_surface.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class Window;
    class SettingsDocument;


    namespace Graphics
    {
        class Image;


        //! Base class/factory for graphics backend - an object that should do necessary API initialization
        //! for graphics-related functions
        class GraphicsBackend
        {
            KMP_DISABLE_COPY_MOVE(GraphicsBackend)
            KMP_LOG_CLASSNAME(GraphicsBackend)

        protected:
            static constexpr auto SettingsEntryName = "GraphicsBackend";
            static constexpr auto MSAAsamplesStr = "MSAAsamples";

        public:
            KMP_NODISCARD KMP_API static UPtr<GraphicsBackend> Create(Window& window);

        public:
            KMP_API explicit GraphicsBackend(Window& window);
            virtual ~GraphicsBackend() = default;

            KMP_NODISCARD KMP_API GraphicsBackendType GetType() const noexcept;

            KMP_NODISCARD KMP_API virtual const GraphicsSurface& GetGraphicsSurface() const noexcept = 0;
            KMP_NODISCARD KMP_API virtual const PhysicalDevice& GetPhysicalDevice() const noexcept = 0;
            KMP_NODISCARD KMP_API virtual PhysicalDevice& GetPhysicalDevice() noexcept = 0;

            KMP_API virtual void StartFrame(float frameTimestep) = 0;
            KMP_API virtual void EndFrame() = 0;
            KMP_API virtual void HandleWindowResize() = 0;

            KMP_NODISCARD KMP_API virtual Nullable<Texture*> CreateTexture(const Filepath& filepath, bool flipVertically = false);
            KMP_NODISCARD KMP_API virtual Nullable<Texture*> CreateTexture(const Image& image) = 0;

            KMP_NODISCARD KMP_API virtual UInt32 GetMultisampling() const = 0;
            KMP_API virtual void SetMultisampling(UInt32 samples) = 0;

            KMP_API virtual void SaveSettings(SettingsDocument& settings) const = 0;
            KMP_API virtual void LoadSettings(SettingsDocument& settings) = 0;

        protected:
            Window& _window;
        };
        //--------------------------------------------------------------------------
    }
}