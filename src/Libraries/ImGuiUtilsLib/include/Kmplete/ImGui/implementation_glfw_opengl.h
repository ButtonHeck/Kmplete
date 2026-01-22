#pragma once

#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Profile/profiler.h"


struct GLFWwindow;


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //! Implementation of ImGui that uses both GLFW as window backend and OpenGL as graphics API
        //! @see ImGuiUtils::ImGuiImplementation
        class ImGuiImplementationGlfwOpenGL : public ImGuiImplementation
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(ImGuiImplementationGlfwOpenGL)

        public:
            ImGuiImplementationGlfwOpenGL(NonNull<GLFWwindow*> window, bool dockingEnabled, bool viewportsEnabled, const char* configName = ConfigurationFileName);
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