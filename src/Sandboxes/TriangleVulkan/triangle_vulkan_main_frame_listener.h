#pragma once

#include "events.h"

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/orthographic_camera.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Input/input_manager.h"

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
        MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, Input::InputManager* inputManager);
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
        bool _OnMultisamplingChangeEvent(Events::MultisamplingChangeEvent& evt);
        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);

    private:
        struct MatrixShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
            Math::Mat4 modelMatrix;
        };

        struct ShaderData
        {
            float colorMultiplier;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        UPtr<Graphics::VulkanVertexBuffer> _vertexBuffer;
        UPtr<Graphics::VulkanBuffer> _indexBuffer;
        Vector<UPtr<Graphics::VulkanUniformBuffer>> _uniformBuffers;
        Vector<UPtr<Graphics::VulkanUniformBuffer>> _matrixUniformBuffers;
        UInt32 _indexCount;
        VkDevice _device;
        VkCommandBuffer _commandBuffer;

        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;

        Events::EventHandlerGuard<Events::MultisamplingChangeEvent> _multisamplingChangeHandler;
        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        MatrixShaderData _matrixShaderData;
        ShaderData _shaderData;
        Graphics::OrthographicCamera _camera;
    };
    //--------------------------------------------------------------------------
}