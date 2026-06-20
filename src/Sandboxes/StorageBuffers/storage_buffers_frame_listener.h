#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/perspective_camera.h"
#include "Kmplete/Event/event_handler_guard.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Input/input_manager.h"
#include "Kmplete/Time/timer.h"
#include "Kmplete/Utils/memory_utils.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class StorageBuffersFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(StorageBuffersFrameListener)

    public:
        static constexpr auto InstancesCount = 1000;
        static constexpr auto ColorsInstancesCount = 100;

    public:
        StorageBuffersFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Input::InputManager* inputManager);
        ~StorageBuffersFrameListener() = default;

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeCamera();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializeStorageBuffers(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnMouseButtonPressedEvent(Events::MouseButtonPressEvent& evt);
        bool _OnMouseScrollEvent(Events::MouseScrollEvent& evt);

    private:
        struct MatricesShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
            Array<Math::Mat4, InstancesCount> models;
        };
        struct ColorShaderData
        {
            ColorShaderData(std::size_t size, std::size_t alignment)
                : color((Math::Vec4F*)Utils::AlignedAlloc(size, alignment))
            {}

            ~ColorShaderData()
            {
                Utils::AlignedFree(color);
            }

            Math::Vec4F* color = nullptr;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        UInt32 _indexCount;
        MatricesShaderData _matricesShaderData;
        UPtr<ColorShaderData> _colorsShaderData;
        size_t _dynamicAlignment;
        Time::Timer _colorRandomizingTimer;
        Array<UInt32, ColorsInstancesCount> _colorsIndices;
        Graphics::PerspectiveCamera _camera;

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::MouseButtonPressEvent> _mouseButtonPressedHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
    };
    //--------------------------------------------------------------------------
}