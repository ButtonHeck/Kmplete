#include "text_rendering_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/Command/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"
#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/ImGui/implementation_glfw_vulkan.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Assets/font_asset_manager.h"
#include "Kmplete/Assets/font_asset.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb.h>
#include <hb-ft.h>


namespace Kmplete
{
    using namespace Graphics::VKBits;

    struct Character
    {
        Math::Vec2F uvMin;
        Math::Vec2F uvMax;
        Math::Vec2I size;
        Math::Vec2I bearing;
        UInt32 advance;
    };

    Map<wchar_t, Character> cyrillicAlphabet;
    const wchar_t cyrillicStart = 0x0400;
    const wchar_t cyrillicEnd = 0x04FF;


    TextRenderingFrameListener::TextRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, 
                                                           Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _localizationManager(localizationManager)
        , _imguiImpl(nullptr)
        , _windowContentScaleHandler(_eventDispatcher, KMP_BIND(TextRenderingFrameListener::_OnWindowContentScaleEvent))
    {
        _FillDictionary();
        _localizationManager.AddLocaleChangedCallback(KMP_BIND(TextRenderingFrameListener::_FillDictionary));

        _Initialize();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_Initialize()
    {
        auto& vulkanPhysicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = vulkanPhysicalDevice.GetLogicalDevice();

        _InitializeBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
        _InitializeImGui();

        _DebugPrint();
        _TestCreateFontAtlas();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        (void)vulkanDevice;
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        (void)vulkanDevice;
        (void)vulkanContext;
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializeImGui()
    {
        const auto isFramebufferAutoScaled = _mainWindow.IsWaylandWindow() || _mainWindow.IsCocoaWindow();
        const auto contentScale = isFramebufferAutoScaled ? 1.0f : _mainWindow.GetContentScale();
        const auto fontDensity = _mainWindow.GetContentScale();

        ImGuiUtils::Context* context = nullptr;
        if (_graphicsBackend.GetType() == Graphics::GraphicsBackendType::Vulkan)
        {
            const auto& vulkanBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
            auto& physicalDevice = dynamic_cast<Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
            auto& logicalDevice = dynamic_cast<Graphics::VulkanLogicalDevice&>(physicalDevice.GetLogicalDevice());

            logicalDevice.GetDescriptorSetManager().AllocateAuxDescriptorPool("ImGui_Pool"_sid, 100, {
                { VK_DescriptorType_Sampler, 100 },
                { VK_DescriptorType_CombinedImageSampler, 100 },
                { VK_DescriptorType_SampledImage, 100 },
                { VK_DescriptorType_StorageImage, 100 },
                { VK_DescriptorType_UniformTexelBuffer, 100 },
                { VK_DescriptorType_StorageTexelBuffer, 100 },
                { VK_DescriptorType_UniformBuffer, 100 },
                { VK_DescriptorType_StorageBuffer, 100 },
                { VK_DescriptorType_UniformBufferDynamic, 100 },
                { VK_DescriptorType_StorageBufferDynamic, 100 },
                { VK_DescriptorType_InputAttachment, 100 }
                });

            const auto viewportEnabled = not _mainWindow.IsWaylandWindow();

            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = vulkanBackend.GetVkInstance();
            initInfo.PhysicalDevice = physicalDevice.GetVkPhysicalDevice();
            initInfo.Device = logicalDevice.GetVkDevice();
            initInfo.QueueFamily = physicalDevice.GetVulkanContext().graphicsFamilyIndex;
            initInfo.Queue = logicalDevice.GetGraphicsQueue().GetVkQueue();
            initInfo.PipelineCache = VK_NULL_HANDLE;
            initInfo.DescriptorPool = logicalDevice.GetDescriptorSetManager().GetAuxDescriptorPool("ImGui_Pool"_sid);
            initInfo.Allocator = VK_NULL_HANDLE;
            initInfo.MinImageCount = Graphics::NumConcurrentFrames;
            initInfo.ImageCount = Graphics::NumConcurrentFrames;
            initInfo.CheckVkResultFn = nullptr;
            initInfo.UseDynamicRendering = true;
            initInfo.MSAASamples = logicalDevice.GetMultisampling();
            initInfo.PipelineRenderingCreateInfo = Graphics::VKUtils::InitVkPipelineRenderingCreateInfoKHR();
            initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
            initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &physicalDevice.GetVulkanContext().surfaceFormatLinear.format;
            initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            initInfo.PipelineRenderingCreateInfo.stencilAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            context = new ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), Graphics::GraphicsBackendTypeToString(_graphicsBackend.GetType()), "docking"_false, viewportEnabled, contentScale, fontDensity, initInfo);
            context->configName = "TextRendering_imgui.ini";
        }
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(context));

        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), 15);
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_DebugPrint()
    {
        const auto domainSid = ToStringID(KMP_TR_DOMAIN_TEXT_RENDERING);
        const auto& alphabet = _localizationManager.Translation(domainSid, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"_sid);
        KMP_MB_UNUSED const auto wideAlphabet = Utils::NarrowToWide(Utils::Utf8ToNarrow(alphabet));

        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        hb_font_t* hbFont = defaultFontAsset.GetFont().GetHbFont();

        const auto& currentLocale = _localizationManager.GetLocale();
        const auto hbScript = currentLocale == LocaleEnUTF8Keyword ? HB_SCRIPT_LATIN : HB_SCRIPT_CYRILLIC;
        const auto hbLanguage = currentLocale == LocaleEnUTF8Keyword ? "en" : "ru";

        hb_buffer_t* alphabetBuffer = hb_buffer_create();
        hb_buffer_add_utf8(alphabetBuffer, alphabet.c_str(), -1, 0, -1);
        hb_buffer_set_direction(alphabetBuffer, HB_DIRECTION_LTR);
        hb_buffer_set_script(alphabetBuffer, hbScript);
        hb_buffer_set_language(alphabetBuffer, hb_language_from_string(hbLanguage, -1));
        hb_shape(hbFont, alphabetBuffer, nullptr, 0);

        unsigned int glyphCount = 0;
        hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(alphabetBuffer, &glyphCount);
        hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(alphabetBuffer, &glyphCount);

        for (unsigned int i = 0; i < glyphCount; i++)
        {
            KMP_LOG_INFO_FN("Glyph ID: {} | AdvanceX: {} | AdvanceY: {} | OffsetX: {} | OffsetY: {}", glyphInfo[i].codepoint, glyphPos[i].x_advance, glyphPos[i].y_advance, glyphPos[i].x_offset, glyphPos[i].y_offset);
        }

        hb_buffer_destroy(alphabetBuffer);
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_TestCreateFontAtlas()
    {
        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);

        unsigned int atlasWidth = 0;
        unsigned int atlasHeight = 0;
        auto* ftFace = defaultFontAsset.GetFont().GetFtFace();
        for (wchar_t c = cyrillicStart; c <= cyrillicEnd; c++)
        {
            if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
            {
                continue;
            }

            atlasWidth += ftFace->glyph->bitmap.width + 1;
            atlasHeight = std::max(atlasHeight, ftFace->glyph->bitmap.rows);
        }

        Vector<UByte> atlasData(atlasWidth * atlasHeight, 0);
        unsigned int offsetX = 0;
        for (wchar_t c = cyrillicStart; c <= cyrillicEnd; c++)
        {
            if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
            {
                continue;
            }

            FT_Bitmap& bitmap = ftFace->glyph->bitmap;
            for (unsigned int y = 0; y < bitmap.rows; y++)
            {
                for (unsigned int x = 0; x < bitmap.width; x++)
                {
                    unsigned int atlasIndex = y * atlasWidth + (offsetX + x);
                    unsigned int glyphIndex = y * bitmap.width + x;
                    atlasData[atlasIndex] = bitmap.buffer[glyphIndex];
                }
            }

            Character character = {
                .uvMin = { float(offsetX) / float(atlasWidth), 0.0f },
                .uvMax = { float(offsetX + bitmap.width) / float(atlasWidth), float(bitmap.rows) / float(atlasHeight) },
                .size = { int(bitmap.width), int(bitmap.rows) },
                .bearing = { ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top },
                .advance = (unsigned int)ftFace->glyph->advance.x
            };

            cyrillicAlphabet[c] = character;
            offsetX += bitmap.width + 1;
        }
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::Render()
    {
        _RenderTexts();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_RenderTexts()
    {
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_RenderImGui()
    {
    }
    //--------------------------------------------------------------------------

    bool TextRenderingFrameListener::_OnWindowContentScaleEvent(Events::WindowContentScaleEvent&)
    {
        _imguiImpl.reset();
        _InitializeImGui();

        return true;
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_FillDictionary()
    {
        _localizationManager.Translate(KMP_TR_DOMAIN_TEXT_RENDERING, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        _localizationManager.Translate(KMP_TR_DOMAIN_TEXT_RENDERING, "0123456789!@#$%^&*()'~`,./<>+-_=;:?");
    }
    //--------------------------------------------------------------------------
}