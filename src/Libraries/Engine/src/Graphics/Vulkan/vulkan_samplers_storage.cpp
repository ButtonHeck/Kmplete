#include "Kmplete/Graphics/Vulkan/vulkan_samplers_storage.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSamplersStorage::VulkanSamplersStorage(VkDevice device, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : _imageCreatorDelegate(imageCreatorDelegate)
            , _device(device)
        {}
        //--------------------------------------------------------------------------

        VulkanSamplersStorage::~VulkanSamplersStorage() KMP_PROFILING(ProfileLevelAlways)
        {
            for (const auto& [sid, sampler] : _samplers)
            {
                vkDestroySampler(_device, sampler, nullptr);
            }

            _samplers.clear();
        }}
        //--------------------------------------------------------------------------

        VkSampler VulkanSamplersStorage::AddSampler(StringID sid, const VkSamplerCreateInfo& createInfo) KMP_PROFILING(ProfileLevelImportant)
        {
            if (_samplers.contains(sid))
            {
                KMP_LOG_WARN("sampler with sid '{}' already registered", sid);
                return _samplers.at(sid);
            }

            try
            {
                auto sampler = _imageCreatorDelegate.CreateVkSampler(createInfo);
                _samplers[sid] = sampler;
                return sampler;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create sampler with sid '{}' - {}", sid, er.what());
            }

            return VK_NULL_HANDLE;
        }}
        //--------------------------------------------------------------------------

        VkSampler VulkanSamplersStorage::GetSampler(StringID sid) const noexcept
        {
            if (!_samplers.contains(sid))
            {
                KMP_LOG_ERROR("sampler with sid '{}' not found", sid);
                return VK_NULL_HANDLE;
            }

            return _samplers.at(sid);
        }
        //--------------------------------------------------------------------------
    }
}