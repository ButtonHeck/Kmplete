#pragma once

#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Profile/profiler.h"


struct GLFWwindow;


namespace Kmplete
{
    namespace ImGuiUtils
    {
        class ImGuiImplementationGlfwVulkan : public ImGuiImplementation
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(ImGuiImplementationGlfwVulkan)

        public:
            ImGuiImplementationGlfwVulkan(NonNull<GLFWwindow*> window, bool dockingEnabled, bool viewportsEnabled, const char* configName = ConfigurationFileName);
            virtual ~ImGuiImplementationGlfwVulkan();

        private:
            void _Initialize(NonNull<GLFWwindow*> window) const;
            void _Finalize() const;

            void _NewFrameImpl() const override;
            void _RenderImpl() const override;
        };
        //--------------------------------------------------------------------------
    }
}