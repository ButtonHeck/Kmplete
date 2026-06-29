#pragma once

#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_fence.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_samplers_storage.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_metrics_manager.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer_manager.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_attachment_manager.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_manager.h"
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
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        class Image;
        struct VulkanGraphicsParameters;


        //TODO: comments
        class VulkanLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanLogicalDevice)
            KMP_LOG_CLASSNAME(VulkanLogicalDevice)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

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

            KMP_NODISCARD KMP_API bool IsVSync() const noexcept;
            KMP_API void SetVSync(bool vSync);

            KMP_NODISCARD KMP_API const VulkanSwapchain& GetSwapchain() const noexcept override;
            KMP_NODISCARD KMP_API VkDevice GetVkDevice() const noexcept;
            KMP_NODISCARD KMP_API const VulkanQueue& GetGraphicsQueue() const noexcept;
            KMP_NODISCARD KMP_API const VulkanQueue& GetPresentationQueue() const noexcept;
            KMP_NODISCARD KMP_API const VulkanImageCreatorDelegate& GetVulkanImageCreatorDelegate() const noexcept;
            KMP_NODISCARD KMP_API const VulkanRenderer& GetRenderer() const noexcept;
            KMP_NODISCARD KMP_API const VkExtent2D& GetCurrentExtent() const noexcept;
            KMP_NODISCARD KMP_API const VulkanSamplersStorage& GetSamplersStorage() const noexcept;
            KMP_NODISCARD KMP_API const VulkanDescriptorSetManager& GetDescriptorSetManager() const noexcept;
            KMP_NODISCARD KMP_API VulkanDescriptorSetManager& GetDescriptorSetManager() noexcept;
            KMP_NODISCARD KMP_API const VulkanPipelineManager& GetPipelineManager() const noexcept;
            KMP_NODISCARD KMP_API VulkanPipelineManager& GetPipelineManager() noexcept;
            KMP_NODISCARD KMP_API const VulkanTextureAttachmentManager& GetTextureAttachmentManager() const noexcept;
            KMP_NODISCARD KMP_API VulkanTextureAttachmentManager& GetTextureAttachmentManager() noexcept;
            KMP_NODISCARD KMP_API const VulkanShaderManager& GetShaderManager() const noexcept;
            KMP_NODISCARD KMP_API VulkanShaderManager& GetShaderManager() noexcept;
            KMP_NODISCARD KMP_API const VulkanBufferManager& GetBufferManager() const noexcept;
            KMP_NODISCARD KMP_API VulkanBufferManager& GetBufferManager() noexcept;
            KMP_NODISCARD KMP_API const VulkanMetricsManager& GetMetricsManager() const noexcept;
            KMP_NODISCARD KMP_API VulkanMetricsManager& GetMetricsManager() noexcept;

            KMP_NODISCARD KMP_API Nullable<VulkanTexture*> CreateTexture(const Image& image) const override;

        private:
            void _CreateLogicalDeviceObject();
            void _DeleteLogicalDeviceObject();

            void _CreateDeviceQueues();
            void _DeleteDeviceQueues();

            void _CreateImageCreatorDelegate();
            void _DeleteImageCreatorDelegate();

            void _CreateSynchronizationObjects();
            void _DeleteSyncronizationObjects();

            void _CreateSwapchain();
            void _DeleteSwapchain();

            void _CreateDescriptorSetManager();
            void _DeleteDescriptorSetManager();

            void _CreateBufferManager();
            void _DeleteBufferManager();

            void _CreateSamplersStorage();
            void _DeleteSamplersStorage();

            void _CreatePipelineManager();
            void _DeletePipelineManager();

            void _CreateTextureAttachmentManager();
            void _DeleteTextureAttachmentManager();

            void _CreateShaderManager();
            void _DeleteShaderManager();

            void _CreateRenderer();
            void _DeleteRenderer();

            void _CreateMetricsManager();
            void _DeleteMetricsManager();

            KMP_NODISCARD Vector<VkDeviceQueueCreateInfo> _CreateQueueCreateInfos() const;
            KMP_NODISCARD VkExtent2D _UpdateExtent() const;
            void _RecreateSwapchain();

        private:
            const VulkanContext& _vulkanContext;
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;
            const VulkanFormatDelegate& _formatDelegate;
            const Window& _window;
            const UInt32& _currentBufferIndex;

            VkPhysicalDevice _physicalDevice;
            VkSurfaceKHR _surface;
            UPtr<VulkanGraphicsParameters> _graphicsParameters;

            VkDevice _device;
            UPtr<VulkanQueue> _graphicsQueue;
            UPtr<VulkanQueue> _presentQueue;
            UPtr<VulkanImageCreatorDelegate> _imageCreatorDelegate;
            Array<VkSemaphore, NumConcurrentFrames> _presentCompleteSemaphores;
            Array<VkSemaphore, NumConcurrentFrames> _renderCompleteSemaphores;
            Vector<VulkanFence> _waitFences;
            UPtr<VulkanSwapchain> _swapchain;
            UPtr<VulkanDescriptorSetManager> _descriptorSetManager;
            UPtr<VulkanBufferManager> _bufferManager;
            VkExtent2D _currentExtent;
            VkSampleCountFlagBits _msaaSamples;
            bool _vSync;
            UPtr<VulkanSamplersStorage> _samplersStorage;
            UPtr<VulkanPipelineManager> _pipelineManager;
            UPtr<VulkanTextureAttachmentManager> _textureAttachmentManager;
            UPtr<VulkanShaderManager> _shaderManager;
            UPtr<VulkanRenderer> _renderer;
            UPtr<VulkanMetricsManager> _metricsManager;
        };
        //--------------------------------------------------------------------------
    }
}