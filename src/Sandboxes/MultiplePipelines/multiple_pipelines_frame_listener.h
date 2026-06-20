#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class MultiplePipelinesFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(MultiplePipelinesFrameListener)

    public:
        MultiplePipelinesFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~MultiplePipelinesFrameListener() = default;

        void Render() override;

    private:
        void _Initialize();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipelines(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
    };
    //--------------------------------------------------------------------------
}