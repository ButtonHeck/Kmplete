#pragma once

#include "triangle_vulkan_main_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class TriangleVulkanApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(TriangleVulkanApplication)
        KMP_DISABLE_COPY_MOVE(TriangleVulkanApplication)

    public:
        explicit TriangleVulkanApplication(const WindowApplicationParameters& parameters);
        ~TriangleVulkanApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<MainFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}