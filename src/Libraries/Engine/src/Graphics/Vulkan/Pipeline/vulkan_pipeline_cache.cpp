#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_pipeline_cache.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <fstream>


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
            KMP_ASSERT(_device);

            bool emptyCache = false;
            if (!Filesystem::FilepathExists(_binaryPath))
            {
                KMP_LOG_WARN("cannot load pipeline cache from '{}' - file not found", _binaryPath);
                emptyCache = true;
            }

            const auto cacheBuffer = Filesystem::ReadFileAsBinary(_binaryPath);
            emptyCache |= cacheBuffer.empty();

            auto pipelineCacheCreateInfo = VKUtils::InitVkPipelineCacheCreateInfo();
            if (emptyCache)
            {
                KMP_LOG_WARN("cannot load pipeline cache from '{}' - file is empty", _binaryPath);
                pipelineCacheCreateInfo.initialDataSize = 0;
                pipelineCacheCreateInfo.pInitialData = nullptr;
            }
            else if (!_LoadedCacheIsValid(cacheBuffer))
            {
                KMP_LOG_WARN("pipeline cache loaded from '{}' is not compatible", _binaryPath);
                pipelineCacheCreateInfo.initialDataSize = 0;
                pipelineCacheCreateInfo.pInitialData = nullptr;
            }
            else
            {
                pipelineCacheCreateInfo.initialDataSize = cacheBuffer.size();
                pipelineCacheCreateInfo.pInitialData = cacheBuffer.data();
            }

            const auto result = vkCreatePipelineCache(_device, &pipelineCacheCreateInfo, nullptr, &_cache);
            VKUtils::CheckResult(result, "VulkanGraphicsPipeline: failed to create pipeline cache", "throw exception"_false);
        }
        //--------------------------------------------------------------------------

        void VulkanPipelineCache::_Finalize()
        {
            KMP_ASSERT(_device);

            if (!_cache)
            {
                KMP_LOG_WARN("cache serialization failed - cache object is null");
                return;
            }

            _SerializeCache();
            _DestroyCache();
        }
        //--------------------------------------------------------------------------

        void VulkanPipelineCache::_SerializeCache() KMP_PROFILING(ProfileLevelImportant)
        {
            size_t cacheSize = 0ULL;
            vkGetPipelineCacheData(_device, _cache, &cacheSize, nullptr);

            BinaryBuffer cacheData(cacheSize);
            vkGetPipelineCacheData(_device, _cache, &cacheSize, cacheData.data());

            std::ofstream outputFile(_binaryPath, std::ios::binary);
            if (!outputFile.is_open())
            {
                KMP_LOG_WARN("cache serialization failed - cannot open file '{}'", _binaryPath);
                return;
            }

            outputFile.write(reinterpret_cast<const char*>(cacheData.data()), cacheSize);
        }}
        //--------------------------------------------------------------------------

        void VulkanPipelineCache::_DestroyCache() KMP_PROFILING(ProfileLevelImportant)
        {
            vkDestroyPipelineCache(_device, _cache, nullptr);
        }}
        //--------------------------------------------------------------------------

        bool VulkanPipelineCache::_LoadedCacheIsValid(const BinaryBuffer& cacheBuffer) const KMP_PROFILING(ProfileLevelImportant)
        {
            const PipelineCacheHeader* cacheHeader = reinterpret_cast<const PipelineCacheHeader*>(cacheBuffer.data());
            
            return cacheHeader->headerVersion == VK_PIPELINE_CACHE_HEADER_VERSION_ONE ||
                   cacheHeader->vendorID == _context.deviceProperties.vendorID ||
                   cacheHeader->deviceID == _context.deviceProperties.deviceID ||
                   memcmp(cacheHeader->pipelineCacheUUID, _context.deviceProperties.pipelineCacheUUID, VK_UUID_SIZE) == 0;
        }}
        //--------------------------------------------------------------------------
    }
}