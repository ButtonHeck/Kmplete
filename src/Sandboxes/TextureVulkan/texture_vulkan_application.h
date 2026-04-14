#pragma once

#include "texture_vulkan_main_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class TextureVulkanApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(TextureVulkanApplication)
        KMP_DISABLE_COPY_MOVE(TextureVulkanApplication)

    public:
        explicit TextureVulkanApplication(const WindowApplicationParameters& parameters);
        ~TextureVulkanApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<MainFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}