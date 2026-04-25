#pragma once

#include "multiple_pipelines_vulkan_main_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class MultiplePipelinesVulkanApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(MultiplePipelinesVulkanApplication)
        KMP_DISABLE_COPY_MOVE(MultiplePipelinesVulkanApplication)

    public:
        explicit MultiplePipelinesVulkanApplication(const WindowApplicationParameters& parameters);
        ~MultiplePipelinesVulkanApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<MainFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}