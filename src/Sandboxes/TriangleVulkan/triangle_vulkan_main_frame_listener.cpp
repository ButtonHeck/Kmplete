#include "triangle_vulkan_main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace
    {
        struct Vertex
        {
            float position[2];
            float color[3];
        };
        //--------------------------------------------------------------------------
    }


    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _vertexBuffer(nullptr)
        , _indexBuffer(nullptr)
        , _indexCount(0)
        , _device(VK_NULL_HANDLE)
        , _descriptorSetLayout(VK_NULL_HANDLE)
        , _pipelineLayout(VK_NULL_HANDLE)
        , _pipeline(VK_NULL_HANDLE)
        , _commandBuffer(VK_NULL_HANDLE)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MainFrameListener::~MainFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Initialize()
    {
        KMP_LOG_DEBUG("initialization started...");

        const Graphics::VulkanPhysicalDevice& vulkanPhysicalDevice = dynamic_cast<const Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        const Graphics::VulkanLogicalDevice& vulkanDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());
        const Graphics::VulkanContext& vulkanContext = vulkanPhysicalDevice.GetVulkanContext();
        _device = vulkanDevice.GetVkDevice();

        // 1. create vertex buffer data
        const Vector<Vertex> vertices{
            {{ 0.95f,  0.95f}, {1.0f, 0.0f, 0.0f}},
            {{-0.95f,  0.97f}, {0.0f, 1.0f, 0.0f}},
            {{-0.92f, -0.95f}, {0.0f, 0.0f, 1.0f}}
        };
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Vertex));


        // 2. create index buffer data
        const Vector<UInt32> indices{ 0, 1, 2 };
        _indexCount = UInt32(indices.size());
        UInt32 indexBufferSize = _indexCount * sizeof(UInt32);


        //3. create staging buffer
        Graphics::VulkanBuffer stagingBuffer = vulkanDevice.CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vertexBufferSize + indexBufferSize);


        // 4. fill staging buffer data
        auto result = stagingBuffer.Map();
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to map texture buffer");
        stagingBuffer.CopyToMappedMemory(0, (char*)vertices.data(), vertexBufferSize);
        stagingBuffer.CopyToMappedMemory(vertexBufferSize, (char*)indices.data(), indexBufferSize);
        result = stagingBuffer.Flush();
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to flush texture buffer");
        stagingBuffer.Unmap();


        // 5. create device-local vertex buffer
        _vertexBuffer.reset(vulkanDevice.CreateBufferPtr(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize));


        // 6. create device-local index buffer
        _indexBuffer.reset(vulkanDevice.CreateBufferPtr(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize));


        {
            // 7. copy staging buffer to device-local vertex and index buffer
            Graphics::VulkanCommandBuffer copyCmd = vulkanDevice.CreateCommandBuffer();
            VkCommandBuffer commandBuffer = copyCmd.GetVkCommandBuffer();
            copyCmd.Begin();

            VkBufferCopy copyRegion{};
            copyRegion.size = vertexBufferSize;
            vkCmdCopyBuffer(commandBuffer, stagingBuffer.GetVkBuffer(), _vertexBuffer->GetVkBuffer(), 1, &copyRegion);
            copyRegion.size = indexBufferSize;
            copyRegion.srcOffset = vertexBufferSize;
            vkCmdCopyBuffer(commandBuffer, stagingBuffer.GetVkBuffer(), _indexBuffer->GetVkBuffer(), 1, &copyRegion);
            copyCmd.End();


            // 8. submit copy command to queue
            Graphics::VulkanFence fence = vulkanDevice.CreateFence(false);
            vulkanDevice.GetGraphicsQueue().Submit(copyCmd, fence.GetVkFence());
            fence.Wait();
        }


        // 9. create empty descriptor set layout
        auto descriptorSetLayoutCI = Graphics::VulkanUtils::InitVkDescriptorSetLayoutCreateInfo();
        descriptorSetLayoutCI.bindingCount = 0;
        descriptorSetLayoutCI.pBindings = nullptr;
        result = vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutCI, nullptr, &_descriptorSetLayout);
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to create descriptor set layout");
        

        // 10. create pipeline layout
        auto pipelineLayoutCI = Graphics::VulkanUtils::InitVkPipelineLayoutCreateInfo();
        pipelineLayoutCI.setLayoutCount = 1;
        pipelineLayoutCI.pSetLayouts = &_descriptorSetLayout;
        result = vkCreatePipelineLayout(_device, &pipelineLayoutCI, nullptr, &_pipelineLayout);
        Graphics::VulkanUtils::CheckResult(result, "MainFrameListener: failed to create pipeline layout");


        // 11.1 begin creating graphics pipeline
        auto pipelineCI = Graphics::VulkanUtils::InitVkGraphicsPipelineCreateInfo();
        pipelineCI.layout = _pipelineLayout;

        // 11.2 input assembly stage
        auto inputAssemblyStateCI = Graphics::VulkanUtils::InitVkPipelineInputAssemblyStateCreateInfo();
        inputAssemblyStateCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        // 11.3 rasterization state
        auto rasterizationStateCI = Graphics::VulkanUtils::InitVkPipelineRasterizationStateCreateInfo();
        rasterizationStateCI.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationStateCI.cullMode = VK_CULL_MODE_NONE;
        rasterizationStateCI.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationStateCI.depthClampEnable = VK_FALSE;
        rasterizationStateCI.rasterizerDiscardEnable = VK_FALSE;
        rasterizationStateCI.depthBiasEnable = VK_FALSE;
        rasterizationStateCI.lineWidth = 1.0f;

        // 11.4 color blend state
        VkPipelineColorBlendAttachmentState blendAttachmentState{};
        blendAttachmentState.colorWriteMask = 0xf;
        blendAttachmentState.blendEnable = VK_FALSE;
        auto colorBlendStateCI = Graphics::VulkanUtils::InitVkPipelineColorBlendStateCreateInfo();
        colorBlendStateCI.attachmentCount = 1;
        colorBlendStateCI.pAttachments = &blendAttachmentState;

        // 11.5 viewport state
        auto viewportStateCI = Graphics::VulkanUtils::InitVkPipelineViewportStateCreateInfo();
        viewportStateCI.viewportCount = 1;
        viewportStateCI.scissorCount = 1;

        // 11.6 dynamic states
        Vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        auto dynamicStateCI = Graphics::VulkanUtils::InitVkPipelineDynamicStateCreateInfo();
        dynamicStateCI.pDynamicStates = dynamicStateEnables.data();
        dynamicStateCI.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

        // 11.7 depth and stencil state
        auto depthStencilStateCI = Graphics::VulkanUtils::InitVkPipelineDepthStencilStateCreateInfo();
        depthStencilStateCI.depthTestEnable = VK_TRUE;
        depthStencilStateCI.depthWriteEnable = VK_TRUE;
        depthStencilStateCI.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        depthStencilStateCI.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateCI.back.failOp = VK_STENCIL_OP_KEEP;
        depthStencilStateCI.back.passOp = VK_STENCIL_OP_KEEP;
        depthStencilStateCI.back.compareOp = VK_COMPARE_OP_ALWAYS;
        depthStencilStateCI.stencilTestEnable = VK_FALSE;
        depthStencilStateCI.front = depthStencilStateCI.back;

        // 11.8 multisampling state
        auto multisampleStateCI = Graphics::VulkanUtils::InitVkPipelineMultisampleStateCreateInfo();
        multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // 11.9 vertex input binding
        VkVertexInputBindingDescription vertexInputBinding{};
        vertexInputBinding.binding = 0;
        vertexInputBinding.stride = sizeof(Vertex);
        vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        Array<VkVertexInputAttributeDescription, 2> vertexInputAttributes{};
        // position
        vertexInputAttributes[0].binding = 0;
        vertexInputAttributes[0].location = 0;
        vertexInputAttributes[0].format = VK_FORMAT_R32G32_SFLOAT;
        vertexInputAttributes[0].offset = offsetof(Vertex, position);
        // color
        vertexInputAttributes[1].binding = 0;
        vertexInputAttributes[1].location = 1;
        vertexInputAttributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributes[1].offset = offsetof(Vertex, color);

        auto vertexInputStateCI = Graphics::VulkanUtils::InitVkPipelineVertexInputStateCreateInfo();
        vertexInputStateCI.vertexBindingDescriptionCount = 1;
        vertexInputStateCI.pVertexBindingDescriptions = &vertexInputBinding;
        vertexInputStateCI.vertexAttributeDescriptionCount = 2;
        vertexInputStateCI.pVertexAttributeDescriptions = vertexInputAttributes.data();

        // 11.10 shaders
        const auto vertexShader = vulkanDevice.CreateShader(String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.vert.spv"));
        const auto fragmentShader = vulkanDevice.CreateShader(String(KMP_SANDBOX_RESOURCES_FOLDER).append("triangle.frag.spv"));
        const auto shaderStages = Vector<VkPipelineShaderStageCreateInfo>{
            vertexShader.GetShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "main"),
            fragmentShader.GetShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "main")
        };

        // 11.11 dynamic rendering
        auto pipelineRenderingCI = Graphics::VulkanUtils::InitVkPipelineRenderingCreateInfoKHR();
        pipelineRenderingCI.colorAttachmentCount = 1;
        pipelineRenderingCI.pColorAttachmentFormats = &vulkanContext.surfaceFormat.format;
        pipelineRenderingCI.depthAttachmentFormat = vulkanContext.defaultDepthFormat;
        pipelineRenderingCI.stencilAttachmentFormat = vulkanContext.defaultDepthFormat;

        // 11.12 assign all previous stages to pipeline creation info
        pipelineCI.pVertexInputState = &vertexInputStateCI;
        pipelineCI.pInputAssemblyState = &inputAssemblyStateCI;
        pipelineCI.pRasterizationState = &rasterizationStateCI;
        pipelineCI.pColorBlendState = &colorBlendStateCI;
        pipelineCI.pMultisampleState = &multisampleStateCI;
        pipelineCI.pViewportState = &viewportStateCI;
        pipelineCI.pDepthStencilState = &depthStencilStateCI;
        pipelineCI.pDynamicState = &dynamicStateCI;
        pipelineCI.stageCount = UInt32(shaderStages.size());
        pipelineCI.pStages = shaderStages.data();
        pipelineCI.pNext = &pipelineRenderingCI;

        // 11.13 create pipeline object
        result = vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCI, nullptr, &_pipeline);

        KMP_LOG_DEBUG("initialization finished");
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Finalize()
    {
        _vertexBuffer.reset();
        _indexBuffer.reset();
        vkDestroyPipeline(_device, _pipeline, nullptr);
        vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
        vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {}
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        const Graphics::VulkanLogicalDevice& vulkanDevice = dynamic_cast<const Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetPhysicalDevice().GetLogicalDevice());

        _commandBuffer = vulkanDevice.GetCurrentCommandBuffer().GetVkCommandBuffer();
        vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);

        VkDeviceSize offsets[1]{ 0 };
        VkBuffer vertexBuffer = _vertexBuffer->GetVkBuffer();
        VkBuffer indexBuffer = _indexBuffer->GetVkBuffer();
        vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &vertexBuffer, offsets);
        vkCmdBindIndexBuffer(_commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(_commandBuffer, _indexCount, 1, 0, 0, 0);
    }
    //--------------------------------------------------------------------------
}