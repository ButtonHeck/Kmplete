#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class MainFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(MainFrameListener)
        KMP_DISABLE_COPY_MOVE(MainFrameListener)

    public:
        MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~MainFrameListener();

        void Render() override;

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        UPtr<Graphics::VulkanVertexBuffer> _vertexBufferInstanced;
        UPtr<Graphics::VulkanBuffer> _indexBuffer;
        UInt32 _indexCount;
        VkDevice _device;
    };
    //--------------------------------------------------------------------------
}