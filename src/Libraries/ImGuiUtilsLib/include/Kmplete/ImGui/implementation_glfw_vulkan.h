#pragma once

#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Profile/profiler.h"


struct GLFWwindow;


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //TODO: comments
        class ImGuiImplementationGlfwVulkan : public ImGuiImplementation
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(ImGuiImplementationGlfwVulkan)

        public:
            explicit ImGuiImplementationGlfwVulkan(ContextVulkan* context);
            virtual ~ImGuiImplementationGlfwVulkan();

            void CreateFontsTexture() const override;

        private:
            void _Initialize() const;
            void _Finalize() const;

            void _NewFrameImpl() const override;
            void _RenderImpl() const override;
        };
        //--------------------------------------------------------------------------
    }
}