#pragma once

#include "events.h"

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Event/window_events.h"


namespace Kmplete
{
    namespace Assets
    {
        class AssetsManager;
    }

    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class PostProcessingFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(PostProcessingFrameListener)

    public:
        PostProcessingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager);
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
        void _InitializeImGui(float dpiScale);

        void _RenderTriangle();
        void _RenderImGui();

        void _SetMultisampling(UInt32 samples);
        bool _OnMultisamplingChangeEvent(Events::MultisamplingChangeEvent& evt);
        bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;

        Events::EventHandlerGuard<Events::MultisamplingChangeEvent> _multisamplingChangeHandler;
        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
    };
    //--------------------------------------------------------------------------
}