#pragma once

#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Profile/profiler.h"


struct GLFWwindow;


namespace Kmplete
{
    namespace ImGuiUtils
    {
        class ImGuiImplementationGlfwOpenGL : public ImGuiImplementation
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_DISABLE_COPY_MOVE(ImGuiImplementationGlfwOpenGL)

            ImGuiImplementationGlfwOpenGL(NonNull<GLFWwindow*> window, bool dockingEnabled, bool viewportsEnabled, const char* configName = "imgui.ini");
            virtual ~ImGuiImplementationGlfwOpenGL();

            void CreateFontsTexture() const override;

        private:
            void _Initialize(NonNull<GLFWwindow*> window) const;
            void _Finalize() const;

            void _NewFrameImpl() const override;
            void _RenderImpl() const override;
        };
        //--------------------------------------------------------------------------
    }
}