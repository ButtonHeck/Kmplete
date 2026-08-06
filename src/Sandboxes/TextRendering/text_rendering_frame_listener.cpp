#include "text_rendering_frame_listener.h"

#include "Kmplete/Application/application_context.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Graphics/font_character.h"
#include "Kmplete/Graphics/image.h"
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


namespace Kmplete
{
    static constexpr auto FontDSLayout_SID = "FontAtlasLayout_DS"_sid;
    static constexpr auto FontDS_SID = "FontAtlas_Set"_sid;

    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "TextRendering_Pipeline"_sid;

    static constexpr auto VertexShader_SID = "TextRendering_vertex"_sid;
    static constexpr auto FragmentShader_SID = "TextRendering_fragment"_sid;

    static constexpr auto TextureFontAtlas_SID = "texture_font_atlas"_sid;

    static constexpr auto SamplerBindingIndex = 0;
    static constexpr auto TextureBindingIndex = 1;

    static constexpr auto VertexBufferBinding = 0;

    static constexpr auto VertexPositionAttributeIndex = 0;
    static constexpr auto VertexUVAttributeIndex = 1;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffer"_sid;
    static constexpr auto UniformBuffers_SID = "uniform_buffers"_sid;


    using namespace Graphics::VKBits;

    Map<wchar_t, Graphics::FontCharacter> cyrillicLocaleCharactersMap;
    Vector<wchar_t> cyrillicLocaleCodes;

    void InitializeCyrillicLocaleCodes()
    {
        static constexpr wchar_t cyrillicStart = 0x0410;
        static constexpr wchar_t cyrillicEnd = 0x044F;
        static constexpr wchar_t cyrillicCapitalIo = 0x0401;
        static constexpr wchar_t cyrillicSmallIo = 0x0451;

        static constexpr wchar_t basicLatinSymbolsStart = 0x0020;
        static constexpr wchar_t basicLatinSymbolsEnd = 0x0040;
        static constexpr wchar_t basicLatinCapitalLettersStart = 0x0041;
        static constexpr wchar_t basicLatinCapitalLettersEnd = 0x005A;
        static constexpr wchar_t basicLatinSmallLettersStart = 0x0061;
        static constexpr wchar_t basicLatinSmallLettersEnd = 0x007A;
        static constexpr wchar_t basicLatinSymbols2Start = 0x005B;
        static constexpr wchar_t basicLatinSymbols2End = 0x0060;
        static constexpr wchar_t basicLatinSymbols3Start = 0x007B;
        static constexpr wchar_t basicLatinSymbols3End = 0x007E;

        for (wchar_t c = cyrillicStart; c <= cyrillicEnd; c++)
        {
            cyrillicLocaleCodes.push_back(c);
        }
        cyrillicLocaleCodes.push_back(cyrillicCapitalIo);
        cyrillicLocaleCodes.push_back(cyrillicSmallIo);

        for (wchar_t c = basicLatinSymbolsStart; c <= basicLatinSymbolsEnd; c++)
        {
            cyrillicLocaleCodes.push_back(c);
        }
        for (wchar_t c = basicLatinCapitalLettersStart; c <= basicLatinCapitalLettersEnd; c++)
        {
            cyrillicLocaleCodes.push_back(c);
        }
        for (wchar_t c = basicLatinSmallLettersStart; c <= basicLatinSmallLettersEnd; c++)
        {
            cyrillicLocaleCodes.push_back(c);
        }
        for (wchar_t c = basicLatinSymbols2Start; c <= basicLatinSymbols2End; c++)
        {
            cyrillicLocaleCodes.push_back(c);
        }
        for (wchar_t c = basicLatinSymbols3Start; c <= basicLatinSymbols3End; c++)
        {
            cyrillicLocaleCodes.push_back(c);
        }
    }
    //--------------------------------------------------------------------------

    void CalculateAtlasSize(FT_FaceRec_* ftFace, unsigned int& atlasWidth, unsigned int& atlasHeight)
    {
        for (const auto& c : cyrillicLocaleCodes)
        {
            if ((FT_Load_Char(ftFace, c, FT_LOAD_RENDER) != FT_Err_Ok) || (FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_SDF) != FT_Err_Ok))
            {
                continue;
            }

            atlasWidth += ftFace->glyph->bitmap.width + 1;
            atlasHeight = std::max(atlasHeight, ftFace->glyph->bitmap.rows);
        }
    }
    //--------------------------------------------------------------------------

    BinaryBuffer CalculateAtlasData(FT_FaceRec_* ftFace, unsigned int atlasWidth, unsigned int atlasHeight)
    {
        BinaryBuffer atlasData(atlasWidth * atlasHeight, 0);
        unsigned int offsetX = 0;

        for (const auto& c : cyrillicLocaleCodes)
        {
            if ((FT_Load_Char(ftFace, c, FT_LOAD_RENDER) != FT_Err_Ok) || (FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_SDF) != FT_Err_Ok))
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

            Graphics::FontCharacter character = {
                .uvMin = { float(offsetX) / float(atlasWidth), 0.0f },
                .uvMax = { float(offsetX + bitmap.width) / float(atlasWidth), float(bitmap.rows) / float(atlasHeight) },
                .size = { int(bitmap.width), int(bitmap.rows) },
                .bearing = { ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top },
                .advance = (unsigned int)ftFace->glyph->advance.x
            };

            cyrillicLocaleCharactersMap[c] = character;
            offsetX += bitmap.width + 1;
        }

        return atlasData;
    }
    //--------------------------------------------------------------------------

    Graphics::Image GenerateTextureAtlas(FT_FaceRec_* ftFace)
    {
        unsigned int atlasWidth = 0;
        unsigned int atlasHeight = 0;
        CalculateAtlasSize(ftFace, atlasWidth, atlasHeight);
        const auto atlasData = CalculateAtlasData(ftFace, atlasWidth, atlasHeight);

        return Graphics::Image(atlasData.data(), int(atlasData.size()), { atlasWidth, atlasHeight }, Graphics::ImageChannels::Grey);
    }
    //--------------------------------------------------------------------------

    Vector<Graphics::FontCharacterVertex> GenerateTextVertices(const WString& text, float x, float y, float scale, float screenWidth, float screenHeight)
    {
        Vector<Graphics::FontCharacterVertex> vertices;
        vertices.reserve(6);

        for (const auto& c : text)
        {
            if (cyrillicLocaleCharactersMap.find(c) == cyrillicLocaleCharactersMap.end())
            {
                continue;
            }

            const auto& ch = cyrillicLocaleCharactersMap[c];
            float xpos = x + ch.bearing.x * scale;
            float ypos = y + (ch.size.y - ch.bearing.y) * scale;
            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            auto toNDC = [&](float px, float py) {
                return Math::Vec2F((px / screenWidth) * 2.0f - 1.0f, (py / screenHeight) * 2.0f - 1.0f);
            };

            const auto p1 = toNDC(xpos, ypos - h);
            const auto p2 = toNDC(xpos, ypos);
            const auto p3 = toNDC(xpos + w, ypos);
            const auto p4 = toNDC(xpos + w, ypos - h);

            vertices.push_back({ p1, Math::Vec2F(ch.uvMin.x, ch.uvMin.y) });
            vertices.push_back({ p2, Math::Vec2F(ch.uvMin.x, ch.uvMax.y) });
            vertices.push_back({ p3, Math::Vec2F(ch.uvMax.x, ch.uvMax.y) });

            vertices.push_back({ p1, Math::Vec2F(ch.uvMin.x, ch.uvMin.y) });
            vertices.push_back({ p3, Math::Vec2F(ch.uvMax.x, ch.uvMax.y) });
            vertices.push_back({ p4, Math::Vec2F(ch.uvMax.x, ch.uvMin.y) });

            x += (ch.advance >> 6) * scale;
        }

        return vertices;
    }
    //--------------------------------------------------------------------------


    TextRenderingFrameListener::TextRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, 
                                                           Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _localizationManager(localizationManager)
        , _imguiImpl(nullptr)
        , _verticesCount(0)
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

        InitializeCyrillicLocaleCodes();
        _TestCreateFontAtlas();
        _InitializeBuffers(vulkanDevice);
        _InitializeUniformBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
        _InitializeImGui();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_TestCreateFontAtlas()
    {
        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        Graphics::Image atlasImage = GenerateTextureAtlas(defaultFontAsset.GetFont().GetFtFace());
        const auto subTypeMask = Assets::TextureSubTypeMaskBits(Assets::TextureSubTypeMaskBits::RGB | Assets::TextureSubTypeMaskBits::NoMipmap);
        const auto atlasTextureCreated = _assetsManager.GetTextureAssetManager().CreateAsset(TextureFontAtlas_SID, atlasImage, subTypeMask);
        KMP_ASSERT(atlasTextureCreated);
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        const auto domainSid = ToStringID(KMP_TR_DOMAIN_TEXT_RENDERING);
        const auto& alphabet = _localizationManager.Translation(domainSid, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"_sid);
        const auto wideAlphabet = Utils::NarrowToWide(Utils::Utf8ToNarrow(alphabet));

        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanRenderer = vulkanDevice.GetRenderer();

        const auto windowFramebufferSize = _mainWindow.GetFramebufferSize();
        const auto vertices = GenerateTextVertices(wideAlphabet, 100.0f, 100.0f, 1.0f, float(windowFramebufferSize.x), float(windowFramebufferSize.y));
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Graphics::FontCharacterVertex));
        _verticesCount = UInt32(vertices.size());

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer({ VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, vertexBufferSize });
        stagingBuffer.Map();
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.Unmap("flush"_true);

        vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
        auto vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        vertexBuffer->AddLayout(Graphics::BufferLayout{
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex },
            Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexUVAttributeIndex }
        });

        vulkanRenderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        auto& textureAssetManager = _assetsManager.GetTextureAssetManager();
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& samplersStorage = vulkanDevice.GetSamplersStorage();

        VkDescriptorSetLayoutBinding samplerLayoutBinding{ SamplerBindingIndex, VK_DescriptorType_Sampler, 1, VK_ShaderStage_Fragment };
        VkDescriptorSetLayoutBinding textureLayoutBinding{ TextureBindingIndex, VK_DescriptorType_SampledImage, 1, VK_ShaderStage_Fragment };
        const auto fontRenderingLayout = descriptorSetManager.AddDescriptorSetLayout(FontDSLayout_SID, { samplerLayoutBinding, textureLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(fontRenderingLayout, FontDS_SID, 1, "per frame"_true);

        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            descriptorSetManager.SetSamplerDescriptor(FontDS_SID, 0, "per frame"_true, i, samplersStorage.GetSampler(Graphics::SamplerDefaultLinearSid), SamplerBindingIndex);
            descriptorSetManager.SetSampledImageDescriptor(
                FontDS_SID, 0, "per frame"_true, i,
                dynamic_cast<Graphics::VulkanTexture&>(textureAssetManager.GetAsset(TextureFontAtlas_SID).GetTexture()).GetVkImageView(), TextureBindingIndex
            );
        }
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormatLinear.format, VK_ImageUsage_TransientAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayoutWithSetsSids(PipelineLayout_SID, { FontDSLayout_SID });

        auto& shaderManager = vulkanDevice.GetShaderManager();
        const auto vertexShaderPath = Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/text_rendering.vert.spv");
        const auto fragmentShaderPath = Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/text_rendering.frag.spv");
        shaderManager.AddShaderModules({
            { VertexShader_SID, vertexShaderPath, Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Vertex },
            { FragmentShader_SID, fragmentShaderPath, Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Fragment }
        });
        const auto shaderStages = shaderManager.GetShaderStageCreateInfos({
            { VertexShader_SID, VK_ShaderStage_Vertex, "main" },
            { FragmentShader_SID, VK_ShaderStage_Fragment, "main" }
        });

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormatLinear.format, Graphics::VKPresets::ColorBlendAttachmentState_AlphaBlending);
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID), VertexBufferBinding);
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "text_rendering_pipeline_cache.bin");
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

    void TextRenderingFrameListener::Render()
    {
        _RenderTexts();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_RenderTexts()
    {
        auto& vulkanGraphicsBackend = dynamic_cast<Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetPhysicalDevice().GetLogicalDevice();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = Graphics::VKUtils::CreateViewport(_mainWindow);
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer() }, { 0 });

        auto colorImageBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_ColorAttachment_PrepareWriting;
        renderer.InsertImageMemoryBarrier(vulkanTextureAttachmentManager.GetTextureAttachment(MS_ColorAttachment), colorImageBarrierParameters);

        auto imageBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_DepthStencil_PrepareWriting;
        renderer.InsertImageMemoryBarrier(vulkanTextureAttachmentManager.GetTextureAttachment(MS_DepthStencilAttachment), imageBarrierParameters);

        const auto colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
            MS_ColorAttachment, 0ULL, VK_Resolve_Average, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true, "use swapchain SRGB"_false
        );
        const auto depthStencilAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_DepthStencil_ClearStore,
            MS_DepthStencilAttachment, 0ULL, VK_Resolve_None, VK_ImageLayout_DontCare
        );

        renderer.BeginRendering(drawArea, { colorAttachmentInfo }, depthStencilAttachmentInfo);
        renderer.BindDescriptorSets(PipelineLayout_SID, 0, {
            descriptorSetManager.GetDescriptorSet(FontDS_SID, 0, "per frame"_true)
        });
        renderer.Draw(_verticesCount, 1, 0, 0);
        renderer.EndRendering();
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