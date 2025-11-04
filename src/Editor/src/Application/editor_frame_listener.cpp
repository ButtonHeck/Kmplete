#include "Application/editor_frame_listener.h"
#include "UI/ui_identifiers.h"

#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "EditorFrameListener";
    static constexpr auto MetricsTimeoutStr = "MetricsTimeout";

    EditorFrameListener::EditorFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager)
        : FrameListener(frameListenerManager, "EditorFrameListener"_sid)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("EditorFrameListener::EditorFrameListener(FrameListenerManager&, Window&, GraphicsBackend&, Assets::AssetsManager&, LocalizationManager&, SystemMetricsManager&)")
        , _systemMetricsManager(systemMetricsManager)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _imguiImpl(nullptr)
        , _uiCompositor(CreateUPtr<EditorUICompositor>(_mainWindow, _assetsManager, localizationManager, systemMetricsManager))
        , _metricsTimer(1000)
    {
        Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    EditorFrameListener::~EditorFrameListener()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Initialize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto dpiScale = _mainWindow.GetDPIScale();

        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend.GetType()), true, true));

        AddImGuiFonts(dpiScale);

        _imguiImpl->Stylize(dpiScale);

        _metricsTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Finalize()
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
            _systemMetricsManager.Update(SystemMetricsManager::SystemMetricsUpdateMode::MemoryAndCPU);
        }
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::Render()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        NewFrame();
        {
            BeginApplicationArea();
            {
                BeginMainWorkingArea();
                {
                    ComposeMainArea();

                    ImGuiUtils::StyleVarGuard styleVarGuard({
                        {ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f)}
                    });
                    EndMainWorkingArea();
                }

                BeginStatusBarArea();
                {
                    ComposeStatusBar();
                }
                EndStatusBarArea();
            }
            EndApplicationArea();
        }

        _imguiImpl->Render();
        EndFrame();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::OnEvent(Event& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>(KMP_BIND(EditorFrameListener::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(KMP_BIND(EditorFrameListener::OnWindowFramebufferRefreshEvent));
        dispatcher.Dispatch<WindowContentScaleEvent>(KMP_BIND(EditorFrameListener::OnWindowContentScaleEvent));

        dispatcher.Dispatch<KeyPressEvent>(KMP_BIND(EditorFrameListener::OnKeyPressEvent));
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        return _uiCompositor->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent&)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        Render();
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnWindowContentScaleEvent(WindowContentScaleEvent& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const auto scale = event.GetScale();

        _imguiImpl.reset();
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend.GetType()), true, true));

        AddImGuiFonts(scale);

        _imguiImpl->Stylize(scale);

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorFrameListener::OnKeyPressEvent(KeyPressEvent& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        return _uiCompositor->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::AddImGuiFonts(float scale)
    {
        const auto& defaultFont = _assetsManager.GetFontManager().GetFont("OpenSans-Regular.ttf"_sid);
        _imguiImpl->AddFont(defaultFont.GetBuffer(), scale);

        const auto& iconsFont = _assetsManager.GetFontManager().GetFont("forkawesome-webfont.ttf"_sid);
        _imguiImpl->AddIconsFont(iconsFont.GetBuffer(), scale);
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

    void EditorFrameListener::NewFrame()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        _imguiImpl->NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::BeginApplicationArea() const
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

    void EditorFrameListener::BeginMainWorkingArea() const
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

            ImGui::BeginChild(IdApp_MainWorkingArea, ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - statusBarHeight), false, workingAreaFlags);
        }

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto dockspaceId = ImGui::GetID(IdApp_EditorDockspace);
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::ComposeMainArea()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _uiCompositor->ComposeMainArea();
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndMainWorkingArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::EndChild(); // IdApp_MainWorkingArea
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::BeginStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGuiUtils::StyleVarGuard styleVarGuard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        ImGuiUtils::StyleColorGuard styleColorGuard({ { ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)} });

        static constexpr auto statusBarFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking;
        ImGui::BeginChild(IdApp_StatusBar, ImGui::GetContentRegionAvail(), false, statusBarFlags);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::ComposeStatusBar()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        _uiCompositor->ComposeStatusBar(_metricsTimer);
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndStatusBarArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::EndChild(); // IdApp_StatusBar
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndApplicationArea() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::End(); // IdApp_ApplicationWindow
    }
    //--------------------------------------------------------------------------

    void EditorFrameListener::EndFrame() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------
}