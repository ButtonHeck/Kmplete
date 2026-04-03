#include "Kmplete/ImGui/implementation_glfw_vulkan.h"
#include "Kmplete/Base/named_bool.h"

#include <GLFW/glfw3.h>
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
            , _commandBuffer(VK_NULL_HANDLE)
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

        void ImGuiImplementationGlfwVulkan::AddTexture(StringID sid, void* sampler, void* view)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportant);

            VkSampler vulkanSampler = reinterpret_cast<VkSampler>(sampler);
            VkImageView vulkanImageView = reinterpret_cast<VkImageView>(view);

            _textureMap[sid] = ImGui_ImplVulkan_AddTexture(vulkanSampler, vulkanImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::RemoveTexture(StringID sid)
        {
            _textureMap.erase(sid);
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::SetCommandBuffer(VkCommandBuffer commandBuffer)
        {
            _commandBuffer = commandBuffer;
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_Initialize() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto window = reinterpret_cast<GLFWwindow*>(_context->window);
            ImGui_ImplGlfw_InitForVulkan(window, "install callbacks"_true);

            const auto contextVulkan = dynamic_cast<ContextVulkan*>(_context.get());
            ImGui_ImplVulkan_InitInfo initInfo = contextVulkan->initInfo;

            ImGui_ImplVulkan_Init(&initInfo);
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_Finalize()
        {
            _textureMap.clear();

            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_NewFrameImpl() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportant);

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }
        //--------------------------------------------------------------------------

        void ImGuiImplementationGlfwVulkan::_RenderImpl() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), _commandBuffer);

            const auto& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }
        //--------------------------------------------------------------------------
    }
}