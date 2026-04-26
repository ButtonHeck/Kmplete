#pragma once

#include "instanced_rendering_vulkan_main_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class InstancedRenderingVulkanApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(InstancedRenderingVulkanApplication)
        KMP_DISABLE_COPY_MOVE(InstancedRenderingVulkanApplication)

    public:
        explicit InstancedRenderingVulkanApplication(const WindowApplicationParameters& parameters);
        ~InstancedRenderingVulkanApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<MainFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}