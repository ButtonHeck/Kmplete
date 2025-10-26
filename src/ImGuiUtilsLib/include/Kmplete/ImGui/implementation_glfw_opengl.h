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

            explicit ImGuiImplementationGlfwOpenGL(NonNull<GLFWwindow*> window);
            virtual ~ImGuiImplementationGlfwOpenGL();

            void NewFrame() override;
            void Render() override;

        private:
            void Initialize(NonNull<GLFWwindow*> window);
            void Finalize();
        };
        //--------------------------------------------------------------------------
    }
}