#pragma once

#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_fence.h"
#include "Kmplete/Graphics/Vulkan/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/vulkan_shader_module.h"
#include "Kmplete/Graphics/Vulkan/vulkan_shader_object.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_samplers_storage.h"
#include "Kmplete/Graphics/Vulkan/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_buffer_creator_delegate.h"
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

            KMP_NODISCARD KMP_API const Swapchain& GetSwapchain() const noexcept override;
            KMP_NODISCARD KMP_API VkDevice GetVkDevice() const noexcept;
            KMP_NODISCARD KMP_API const VulkanQueue& GetGraphicsQueue() const noexcept;
            KMP_NODISCARD KMP_API const VulkanQueue& GetPresentationQueue() const noexcept;
            KMP_NODISCARD KMP_API VkDescriptorPool GetVkDescriptorPool() const noexcept;
            KMP_NODISCARD KMP_API const VulkanImageCreatorDelegate& GetVulkanImageCreatorDelegate() const noexcept;
            KMP_NODISCARD KMP_API const VulkanBufferCreatorDelegate& GetVulkanBufferCreatorDelegate() const noexcept;
            KMP_NODISCARD KMP_API const VulkanRenderer& GetRenderer() const noexcept;
            KMP_NODISCARD KMP_API const VkExtent2D& GetCurrentExtent() const noexcept;
            KMP_NODISCARD KMP_API const VulkanSamplersStorage& GetSamplersStorage() const noexcept;
            KMP_NODISCARD KMP_API VulkanDescriptorSetManager& GetDescriptorSetManager() noexcept;

            KMP_NODISCARD KMP_API VulkanGraphicsPipeline& AddGraphicsPipeline(StringID sid);
            KMP_NODISCARD KMP_API OptionalRef<VulkanGraphicsPipeline> GetGraphicsPipeline(StringID sid) const;

            KMP_NODISCARD KMP_API Nullable<VulkanTexture*> CreateTexture(const Image& image) const override;
            KMP_NODISCARD KMP_API VulkanFence CreateFence(bool signaled = true) const;
            KMP_NODISCARD KMP_API VulkanShaderModule CreateShaderModule(const Filepath& filepath) const;

            KMP_API bool AddShaderObject(StringID sid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                         const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            KMP_API bool AddShaderObject(StringID sid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                         const Vector<StringID>& descriptorSetsLayoutsSids, const char* name = "main");
            KMP_NODISCARD KMP_API VkShaderEXT GetShaderObject(StringID sid) const noexcept;

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

            void _CreatePipelineCache();
            void _DeletePipelineCache();

            void _CreateDescriptorPool();
            void _DeleteDescriptorPool();

            void _DeletePipelines();
            void _DeleteShaderObjects();

            void _CreateBufferCreatorDelegate();
            void _DeleteBufferCreatorDelegate();

            void _CreateRenderer();
            void _DeleteRenderer();

            void _CreateSamplersStorage();
            void _DeleteSamplersStorage();

            void _CreateDescriptorSetManager();
            void _DeleteDescriptorSetManager();

            KMP_NODISCARD Vector<VkDeviceQueueCreateInfo> _CreateQueueCreateInfos() const;
            KMP_NODISCARD VkExtent2D _UpdateExtent() const;

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
            UPtr<VulkanSwapchain> _swapchain;
            VkPipelineCache _pipelineCache;
            VkDescriptorPool _descriptorPool;
            StringIDHashMap<UPtr<VulkanGraphicsPipeline>> _pipelines;
            UPtr<VulkanBufferCreatorDelegate> _bufferCreatorDelegate;
            VkExtent2D _currentExtent;
            VkSampleCountFlagBits _msaaSamples;
            UPtr<VulkanRenderer> _renderer;
            StringIDHashMap<UPtr<VulkanShaderObject>> _shaderObjects;
            UPtr<VulkanSamplersStorage> _samplersStorage;
            UPtr<VulkanDescriptorSetManager> _descriptorSetManager;
        };
        //--------------------------------------------------------------------------
    }
}