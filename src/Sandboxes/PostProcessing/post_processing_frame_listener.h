#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class PostProcessingFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(PostProcessingFrameListener)

    public:
        PostProcessingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~PostProcessingFrameListener() = default;

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
        void _InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
    };
    //--------------------------------------------------------------------------
}