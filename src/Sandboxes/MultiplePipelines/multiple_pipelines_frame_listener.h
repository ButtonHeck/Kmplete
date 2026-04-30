#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class MultiplePipelinesFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(MultiplePipelinesFrameListener)
        KMP_DISABLE_COPY_MOVE(MultiplePipelinesFrameListener)

    public:
        MultiplePipelinesFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~MultiplePipelinesFrameListener();

        void Render() override;

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBufferFixedColor;
        UPtr<Graphics::VulkanVertexBuffer> _vertexBufferBufferedColor;
        VkDevice _device;
    };
    //--------------------------------------------------------------------------
}