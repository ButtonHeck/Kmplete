#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.h"


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class PushConstantsFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(PushConstantsFrameListener)

    public:
        static constexpr auto InstancesCount = 8;

    public:
        PushConstantsFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~PushConstantsFrameListener();

        void Render() override;

    private:
        struct PushConstantsData
        {
            Math::Vec4F position;
            Math::Vec4F color;
        };

    private:
        void _Initialize();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _Finalize();

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        Array<PushConstantsData, InstancesCount> _pushConstantsArray;
    };
    //--------------------------------------------------------------------------
}