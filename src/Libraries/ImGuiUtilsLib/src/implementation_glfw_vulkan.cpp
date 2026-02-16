#include "Kmplete/ImGui/implementation_glfw_vulkan.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ImGuiImplementationGlfwVulkan::ImGuiImplementationGlfwVulkan(ContextVulkan* context)
            : ImGuiImplementation(context)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        {
            _Initialize();
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        ImGuiImplementationGlfwVulkan::~ImGuiImplementationGlfwVulkan()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Finalize();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::CreateFontsTexture() const
        {
            ImGui_ImplVulkan_CreateFontsTexture();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_Initialize() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto window = reinterpret_cast<GLFWwindow*>(_context->window);
            ImGui_ImplGlfw_InitForVulkan(window, true);

            const auto contextVulkan = dynamic_cast<ContextVulkan*>(_context.get());
            ImGui_ImplVulkan_InitInfo initInfo = contextVulkan->initInfo;

            ImGui_ImplVulkan_Init(&initInfo);
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_Finalize() const
        {
            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_NewFrameImpl() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_RenderImpl() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            //ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), )
        }
        //--------------------------------------------------------------------------
    }
}