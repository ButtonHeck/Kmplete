#include "Application/editor_frame_listener.h"
#include "UI/ui_identifiers.h"

#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Input/input_manager.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/ImGui/implementation_glfw_vulkan.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "EditorFrameListener";
    static constexpr auto MetricsTimeoutStr = "MetricsTimeout";


    EditorFrameListener::EditorFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager,
                                             LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager)
        : FrameListener(frameListenerManager, "EditorFrameListener"_sid, 0)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _systemMetricsManager(systemMetricsManager)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _imguiImpl(nullptr)
        , _uiCompositor(nullptr)
        , _metricsTimer(1000)
        , _windowCloseHandler(_eventDispatcher, KMP_BIND(EditorFrameListener::_OnWindowCloseEvent))
        , _windowContentScaleHandler(_eventDispatcher, KMP_BIND(EditorFrameListener::_OnWindowContentScaleEvent))
        , _editorFullscreenHandler(_eventDispatcher, KMP_BIND(EditorFrameListener::_OnEditorFullscreenEvent))
    {
        _Initialize(localizationManager, systemMetricsManager, inputManager);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    EditorFrameListener::~EditorFrameListener()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_Initialize(LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto dpiScale = _mainWindow.GetDPIScale();

        _InitializeImGui(dpiScale);

        _uiCompositor.reset(new EditorUICompositor(_mainWindow, _assetsManager, localizationManager, systemMetricsManager, inputManager, *_imguiImpl.get()));

        _metricsTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_InitializeImGui(float dpiScale)
    {
        ImGuiUtils::Context* context = nullptr;
        if (_graphicsBackend.GetType() == Graphics::GraphicsBackendType::Vulkan)
        {
            const auto& vulkanBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
            const auto& physicalDevice = dynamic_cast<const Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
            const auto& logicalDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(physicalDevice.GetLogicalDevice());

            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = vulkanBackend.GetVkInstance();
            initInfo.PhysicalDevice = physicalDevice.GetVkPhysicalDevice();
            initInfo.Device = logicalDevice.GetVkDevice();
            initInfo.QueueFamily = physicalDevice.GetVulkanContext().graphicsFamilyIndex;
            initInfo.Queue = logicalDevice.GetGraphicsQueue().GetVkQueue();
            initInfo.PipelineCache = VK_NULL_HANDLE;
            initInfo.DescriptorPool = logicalDevice.GetVkDescriptorPool();
            initInfo.Allocator = VK_NULL_HANDLE;
            initInfo.MinImageCount = Graphics::NumConcurrentFrames;
            initInfo.ImageCount = Graphics::NumConcurrentFrames;
            initInfo.CheckVkResultFn = nullptr;
            initInfo.UseDynamicRendering = true;
            initInfo.PipelineRenderingCreateInfo = Graphics::VulkanUtils::InitVkPipelineRenderingCreateInfoKHR();
            initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
            initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &physicalDevice.GetVulkanContext().surfaceFormat.format;
            initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            initInfo.PipelineRenderingCreateInfo.stencilAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            context = new ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), Graphics::GraphicsBackendTypeToString(_graphicsBackend.GetType()), "docking"_true, "viewport"_true, initInfo);

            _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(context));

            auto& flagUSATexture = dynamic_cast<Graphics::VulkanTexture&>(_assetsManager.GetTextureAssetManager().GetAsset("_flag_usa"_sid).GetTexture());
            auto& flagRussiaTexture = dynamic_cast<Graphics::VulkanTexture&>(_assetsManager.GetTextureAssetManager().GetAsset("_flag_russian"_sid).GetTexture());
            _imguiImpl->AddTexture("_flag_usa"_sid, flagUSATexture.GetVkSampler(), flagUSATexture.GetVkImageView());
            _imguiImpl->AddTexture("_flag_russian"_sid, flagRussiaTexture.GetVkSampler(), flagRussiaTexture.GetVkImageView());
        }

        _AddImGuiFonts(dpiScale);
        _imguiImpl->Stylize(dpiScale);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _imguiImpl.reset();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Update(float /*frameTimestep*/, bool /*applicationIsIconified*/)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_metricsTimer.ReachedTimeout())
        {
            _metricsTimer.Mark();
            _systemMetricsManager.Update(SystemMetricsManager::SystemMetricsUpdateMode::MemoryCPUAndStack);
        }
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Render()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _NewFrame();
        {
            _BeginApplicationArea();
            {
                _BeginMainWorkingArea();
                {
                    _ComposeMainArea();

                    ImGuiUtils::StyleVarGuard styleVarGuard({
                        {ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f)}
                    });
                    _EndMainWorkingArea();
                }

                _BeginStatusBarArea();
                {
                    _ComposeStatusBar();
                }
                _EndStatusBarArea();
            }
            _EndApplicationArea();
        }

        if (_graphicsBackend.GetType() == Graphics::GraphicsBackendType::Vulkan)
        {
            auto& vulkanLogicalDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
            auto commandBuffer = vulkanLogicalDevice.GetRenderer().GetCurrentCommandBuffer();
            auto* vulkanImGuiUtils = dynamic_cast<ImGuiUtils::ImGuiImplementationGlfwVulkan*>(_imguiImpl.get());
            vulkanImGuiUtils->SetCommandBuffer(commandBuffer);
            vulkanImGuiUtils->Render();
        }
        else
        {
            _imguiImpl->Render();
        }

        _EndFrame();
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::_OnWindowCloseEvent(Events::WindowCloseEvent& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        return _uiCompositor->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::_OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const auto scale = event.GetScale();

        _imguiImpl.reset();
        _InitializeImGui(scale);

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::_OnEditorFullscreenEvent(Events::EditorFullscreenEvent& event)
    {
        _mainWindow.SetScreenMode(event.screenMode);
        return true;
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_AddImGuiFonts(float scale)
    {
        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset("OpenSans-Regular.ttf"_sid);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), scale);

        const auto& iconsFontAsset = _assetsManager.GetFontAssetManager().GetAsset("forkawesome-webfont.ttf"_sid);
        _imguiImpl->AddIconsFont(iconsFontAsset.GetFont().GetBuffer(), scale);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveUInt(MetricsTimeoutStr, _metricsTimer.GetTimeout());
        _uiCompositor->SaveSettings(settings);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartLoadObject(SettingsEntryName);
        _metricsTimer.SetTimeout(settings.GetUInt(MetricsTimeoutStr, 1000));
        _uiCompositor->LoadSettings(settings);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_NewFrame()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        _imguiImpl->NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_BeginApplicationArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiUtils::StyleVarGuard guard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        static constexpr auto applicationWindowFlags =
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin(IdApp_ApplicationWindow, nullptr, applicationWindowFlags);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_BeginMainWorkingArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowViewport(viewport->ID);

        {
            ImGuiUtils::StyleVarGuard guard({
                {ImGuiStyleVar_WindowRounding, 0.0f},
                {ImGuiStyleVar_WindowBorderSize, 0.0f},
                {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
            });

            const auto statusBarHeight = 28 * _mainWindow.GetDPIScale();
            static constexpr auto workingAreaFlags =
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::BeginChild(IdApp_MainWorkingArea, ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - statusBarHeight), 0, workingAreaFlags);
        }

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto dockspaceId = ImGui::GetID(IdApp_EditorDockspace);
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_ComposeMainArea()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _uiCompositor->ComposeMainArea();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_EndMainWorkingArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::EndChild(); // IdApp_MainWorkingArea
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_BeginStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGuiUtils::StyleVarGuard styleVarGuard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        ImGuiUtils::StyleColorGuard styleColorGuard({ { ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)} });

        static constexpr auto statusBarFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking;
        ImGui::BeginChild(IdApp_StatusBar, ImGui::GetContentRegionAvail(), 0, statusBarFlags);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_ComposeStatusBar()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _uiCompositor->ComposeStatusBar(_metricsTimer);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_EndStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::EndChild(); // IdApp_StatusBar
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_EndApplicationArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::End(); // IdApp_ApplicationWindow
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::_EndFrame() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------
}