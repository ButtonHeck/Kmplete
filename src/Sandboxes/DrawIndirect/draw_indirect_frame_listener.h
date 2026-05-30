#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class DrawIndirectFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(DrawIndirectFrameListener)

    public:
        DrawIndirectFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~DrawIndirectFrameListener();

        void Render() override;

    private:
        void _Initialize();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _Finalize();

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        UPtr<Graphics::VulkanVertexBuffer> _vertexInstanceBuffer;
        UPtr<Graphics::VulkanBuffer> _indexBuffer;
        UPtr<Graphics::VulkanBuffer> _indirectBuffer;
        UInt32 _indexCount;
    };
    //--------------------------------------------------------------------------
}