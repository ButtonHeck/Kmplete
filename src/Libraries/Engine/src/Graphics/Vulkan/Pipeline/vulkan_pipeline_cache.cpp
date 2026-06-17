#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_pipeline_cache.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanPipelineCache::VulkanPipelineCache(VkDevice device, const VulkanContext& context, const Filepath& binaryPath)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _context(context)
            , _cache(VK_NULL_HANDLE)
            , _binaryPath(binaryPath)
        {
            _Initialize();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanPipelineCache::~VulkanPipelineCache() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkPipelineCache VulkanPipelineCache::GetVkPipelineCache() const noexcept
        {
            return _cache;
        }
        //--------------------------------------------------------------------------

        void VulkanPipelineCache::_Initialize()
        {
        }
        //--------------------------------------------------------------------------

        void VulkanPipelineCache::_Finalize()
        {
        }
        //--------------------------------------------------------------------------
    }
}