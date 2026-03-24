#pragma once

#include "events.h"

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Event/event_handler_guard.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Assets
    {
        class AssetsManager;
    }


    class MainFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(MainFrameListener)
        KMP_DISABLE_COPY_MOVE(MainFrameListener)

    public:
        MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager);
        ~MainFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeTriangle();
        void _InitializeImGui(float dpiScale);
        void _Finalize();

        void _RenderTriangle();
        void _RenderImGui();

        void _SetMultisampling(UInt32 samples);
        bool _OnMultisamplingChangeEvent(Events::MultisamplingChangeEvent&);

    private:
        struct ShaderData
        {
            float colorMultiplier;
        };
        //--------------------------------------------------------------------------

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        UPtr<Graphics::VulkanBuffer> _indexBuffer;
        Vector<UPtr<Graphics::VulkanUniformBuffer>> _uniformBuffers;
        UInt32 _indexCount;
        VkDevice _device;
        VkCommandBuffer _commandBuffer;

        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;

        Events::EventHandlerGuard<Events::MultisamplingChangeEvent> _multisamplingChangeHandler;
        ShaderData _shaderData;
    };
    //--------------------------------------------------------------------------
}