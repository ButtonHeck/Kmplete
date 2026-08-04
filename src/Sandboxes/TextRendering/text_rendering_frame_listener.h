#pragma once

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


    class TextRenderingFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(TextRenderingFrameListener)

    public:
        TextRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend,
                                   Assets::AssetsManager& assetsManager);
        ~TextRenderingFrameListener() = default;

        void Render() override;

    private:
        void _Initialize();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializeImGui();

        void _RenderTexts();
        void _RenderImGui();

        bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;

        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
    };
    //--------------------------------------------------------------------------
}