#pragma once

#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_fence.h"
#include "Kmplete/Graphics/Vulkan/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/vulkan_shader.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        class Image;


        //TODO: comments
        class VulkanLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanLogicalDevice)
            KMP_LOG_CLASSNAME(VulkanLogicalDevice)

        public:
            KMP_API VulkanLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VulkanContext& vulkanContext, const VulkanMemoryTypeDelegate& memoryTypeDelegate,
                                        const VulkanFormatDelegate& formatDelegate, const Window& window, const UInt32& currentBufferIndex);
            KMP_API ~VulkanLogicalDevice();

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;
            KMP_API void HandleWindowResize() override;
            KMP_API void WaitIdle() const;

            KMP_NODISCARD KMP_API VkSampleCountFlagBits GetMultisampling() const noexcept;
            KMP_API void SetMultisampling(VkSampleCountFlagBits samples);

            KMP_NODISCARD KMP_API const CommandPool& GetCommandPool() const noexcept override;
            KMP_NODISCARD KMP_API const Swapchain& GetSwapchain() const noexcept override;
            KMP_NODISCARD KMP_API VkDevice GetVkDevice() const noexcept;
            KMP_NODISCARD KMP_API const VulkanQueue& GetGraphicsQueue() const noexcept;
            KMP_NODISCARD KMP_API const VulkanQueue& GetPresentationQueue() const noexcept;
            KMP_NODISCARD KMP_API VkDescriptorPool GetVkDescriptorPool() const noexcept;
            KMP_NODISCARD KMP_API const VulkanCommandBuffer& GetCurrentCommandBuffer() const noexcept;
            KMP_NODISCARD KMP_API const VulkanImageCreatorDelegate& GetVulkanImageCreatorDelegate() const noexcept;

            KMP_NODISCARD KMP_API VulkanGraphicsPipeline& AddGraphicsPipeline(StringID sid);
            KMP_NODISCARD KMP_API OptionalRef<VulkanGraphicsPipeline> GetGraphicsPipeline(StringID sid) const;

            KMP_NODISCARD KMP_API Nullable<VulkanTexture*> CreateTexture(const Image& image) const override;
            KMP_NODISCARD KMP_API VulkanBuffer CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;
            KMP_NODISCARD KMP_API VulkanVertexBuffer CreateVertexBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;
            KMP_NODISCARD KMP_API VulkanUniformBuffer CreateUniformBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> CreateVertexBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;
            KMP_NODISCARD KMP_API Nullable<VulkanUniformBuffer*> CreateUniformBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const;
            KMP_NODISCARD KMP_API VulkanCommandBuffer CreateCommandBuffer() const;
            KMP_NODISCARD KMP_API VulkanFence CreateFence(bool signaled = true) const;
            KMP_NODISCARD KMP_API VulkanShader CreateShader(const Filepath& filepath) const;

        private:
            void _CreateLogicalDeviceObject();
            void _CreateDeviceQueues();
            void _DeleteLogicalDeviceObject();

            void _CreateSynchronizationObjects();
            void _DeleteSyncronizationObjects();

            void _CreateCommandPool();
            void _DeleteCommandPool();

            void _CreateSwapchain();
            void _DeleteSwapchain();

            void _CreateCommandBuffers();
            void _DeleteCommandBuffers();

            void _CreatePipelineCache();
            void _DeletePipelineCache();

            void _CreateDescriptorPool();
            void _DeleteDescriptorPool();

            KMP_NODISCARD Vector<VkDeviceQueueCreateInfo> _CreateQueueCreateInfos() const;
            KMP_NODISCARD VkExtent2D _UpdateExtent() const;

            void _StartFrameRestartFence();
            void _StartFrameRestartCommandBuffer();
            void _StartFrameTransitionColorAndDepthStencilImages();
            void _StartFrameBeginRendering();
            void _StartFrameSetupViewportAndScissor();
            void _StartFrameSetupRasterizationSamples();

            void _EndFrameEndRendering();
            void _EndFrameTransitionColorAndDepthStencilImages();
            void _EndFrameEndCommandBuffer();
            void _EndFrameQueueSubmit();

        private:
            const VulkanContext& _vulkanContext;
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;
            const VulkanFormatDelegate& _formatDelegate;
            const Window& _window;
            const UInt32& _currentBufferIndex;

            VkPhysicalDevice _physicalDevice;
            VkSurfaceKHR _surface;

            VkDevice _device;
            UPtr<VulkanQueue> _graphicsQueue;
            UPtr<VulkanQueue> _presentQueue;
            UPtr<VulkanImageCreatorDelegate> _imageCreatorDelegate;

            Array<VkSemaphore, NumConcurrentFrames> _presentCompleteSemaphores;
            Array<VkSemaphore, NumConcurrentFrames> _renderCompleteSemaphores;
            Vector<VulkanFence> _waitFences;
            UPtr<VulkanCommandPool> _commandPool;
            UPtr<VulkanSwapchain> _swapchain;
            Vector<VulkanCommandBuffer> _drawCommandBuffers;
            VkPipelineCache _pipelineCache;
            VkDescriptorPool _descriptorPool;
            HashMap<StringID, UPtr<VulkanGraphicsPipeline>> _pipelines;

            VkExtent2D _currentExtent;
            VkSampleCountFlagBits _msaaSamples;
        };
        //--------------------------------------------------------------------------
    }
}