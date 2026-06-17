#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanBufferManager::VulkanBufferManager(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _memoryTypeDelegate(memoryTypeDelegate)
            , _buffers()
            , _vertexBuffers()
            , _perFrameBuffers()
            , _perFrameVertexBuffers()
        {
            KMP_ASSERT(_device);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _CreateBuffer(parameters);
        }}
        //--------------------------------------------------------------------------

        bool VulkanBufferManager::CreateBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                if (_perFrameBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("per-frame buffers with sid '{}' have already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _perFrameBuffers.emplace(bufferSid, Array<UPtr<VulkanBuffer>, NumConcurrentFrames>{});
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace '{}' per-frame buffers with sid '{}'", NumConcurrentFrames, bufferSid);
                    return false;
                }

                for (auto i = 0; i < NumConcurrentFrames; i++)
                {
                    _perFrameBuffers[bufferSid][i].reset(_CreateBufferPtr(parameters));
                }

                return true;
            }
            else
            {
                if (_buffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("buffer with sid '{}' has already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _CreateBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanVertexBuffer VulkanBufferManager::CreateVertexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _CreateVertexBuffer(parameters);
        }}
        //--------------------------------------------------------------------------

        bool VulkanBufferManager::CreateVertexBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                if (_perFrameVertexBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("per-frame vertex buffers with sid '{}' have already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _perFrameVertexBuffers.emplace(bufferSid, Array<UPtr<VulkanVertexBuffer>, NumConcurrentFrames>{});
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace '{}' per-frame vertex buffers with sid '{}'", NumConcurrentFrames, bufferSid);
                    return false;
                }

                for (auto i = 0; i < NumConcurrentFrames; i++)
                {
                    _perFrameVertexBuffers[bufferSid][i].reset(_CreateVertexBufferPtr(parameters));
                }

                return true;
            }
            else
            {
                if (_vertexBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("vertex buffer with sid '{}' has already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _vertexBuffers.emplace(bufferSid, _CreateVertexBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateIndexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _CreateIndexBuffer(parameters);
        }}
        //--------------------------------------------------------------------------

        bool VulkanBufferManager::CreateIndexBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                if (_perFrameBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("per-frame index buffers with sid '{}' have already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _perFrameBuffers.emplace(bufferSid, Array<UPtr<VulkanBuffer>, NumConcurrentFrames>{});
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace '{}' per-frame index buffers with sid '{}'", NumConcurrentFrames, bufferSid);
                    return false;
                }

                for (auto i = 0; i < NumConcurrentFrames; i++)
                {
                    _perFrameBuffers[bufferSid][i].reset(_CreateIndexBufferPtr(parameters));
                }

                return true;
            }
            else
            {
                if (_buffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("index buffer with sid '{}' has already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _CreateIndexBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateUniformBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _CreateUniformBuffer(parameters);
        }}
        //--------------------------------------------------------------------------

        bool VulkanBufferManager::CreateUniformBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                if (_perFrameBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("per-frame uniform buffers with sid '{}' have already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _perFrameBuffers.emplace(bufferSid, Array<UPtr<VulkanBuffer>, NumConcurrentFrames>{});
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace '{}' per-frame uniform buffers with sid '{}'", NumConcurrentFrames, bufferSid);
                    return false;
                }

                for (auto i = 0; i < NumConcurrentFrames; i++)
                {
                    _perFrameBuffers[bufferSid][i].reset(_CreateUniformBufferPtr(parameters));
                }

                return true;
            }
            else
            {
                if (_buffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("uniform buffer with sid '{}' has already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _CreateUniformBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateStorageBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _CreateStorageBuffer(parameters);
        }}
        //--------------------------------------------------------------------------

        bool VulkanBufferManager::CreateStorageBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                if (_perFrameBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("per-frame storage buffers with sid '{}' have already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _perFrameBuffers.emplace(bufferSid, Array<UPtr<VulkanBuffer>, NumConcurrentFrames>{});
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace '{}' per-frame storage buffers with sid '{}'", NumConcurrentFrames, bufferSid);
                    return false;
                }

                for (auto i = 0; i < NumConcurrentFrames; i++)
                {
                    _perFrameBuffers[bufferSid][i].reset(_CreateStorageBufferPtr(parameters));
                }

                return true;
            }
            else
            {
                if (_buffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("storage buffer with sid '{}' has already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _CreateStorageBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateIndirectBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _CreateIndirectBuffer(parameters);
        }}
        //--------------------------------------------------------------------------

        bool VulkanBufferManager::CreateIndirectBufferPtr(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame /*= false*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                if (_perFrameBuffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("per-frame indirect buffers with sid '{}' have already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _perFrameBuffers.emplace(bufferSid, Array<UPtr<VulkanBuffer>, NumConcurrentFrames>{});
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace '{}' per-frame indirect buffers with sid '{}'", NumConcurrentFrames, bufferSid);
                    return false;
                }

                for (auto i = 0; i < NumConcurrentFrames; i++)
                {
                    _perFrameBuffers[bufferSid][i].reset(_CreateIndirectBufferPtr(parameters));
                }

                return true;
            }
            else
            {
                if (_buffers.contains(bufferSid))
                {
                    KMP_LOG_WARN("indirect buffer with sid '{}' has already been created", bufferSid);
                    return true;
                }

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _CreateIndirectBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        Nullable<VulkanBuffer*> VulkanBufferManager::GetBuffer(StringID bufferSid) const noexcept
        {
            if (_buffers.contains(bufferSid))
            {
                return _buffers.at(bufferSid).get();
            }

            KMP_LOG_ERROR("failed to find buffer with sid '{}'", bufferSid);
            return nullptr;
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferManager::GetBuffer(StringID bufferSid, UInt32 index) const noexcept
        {
            if (index >= NumConcurrentFrames)
            {
                KMP_LOG_ERROR("cannot find per-frame buffer with sid '{}' - index '{}' is out of bounds", bufferSid, index);
                return nullptr;
            }

            if (_perFrameBuffers.contains(bufferSid))
            {
                return _perFrameBuffers.at(bufferSid)[index].get();
            }

            KMP_LOG_ERROR("failed to find per-frame buffer with sid '{}'", bufferSid);
            return nullptr;
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferManager::GetVertexBuffer(StringID bufferSid) const noexcept
        {
            if (_vertexBuffers.contains(bufferSid))
            {
                return _vertexBuffers.at(bufferSid).get();
            }

            KMP_LOG_ERROR("failed to find vertex buffer with sid '{}'", bufferSid);
            return nullptr;
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferManager::GetVertexBuffer(StringID bufferSid, UInt32 index) const noexcept
        {
            if (index >= NumConcurrentFrames)
            {
                KMP_LOG_ERROR("cannot find per-frame vertex buffer with sid '{}' - index '{}' is out of bounds", bufferSid, index);
                return nullptr;
            }

            if (_perFrameVertexBuffers.contains(bufferSid))
            {
                return _perFrameVertexBuffers.at(bufferSid)[index].get();
            }

            KMP_LOG_ERROR("failed to find per-frame vertex buffer with sid '{}'", bufferSid);
            return nullptr;
        }
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::_CreateBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, parameters);
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferManager::_CreateBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, parameters);
        }}
        //--------------------------------------------------------------------------


        VulkanVertexBuffer VulkanBufferManager::_CreateVertexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Vertex | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferManager::_CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Vertex | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::_CreateIndexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Index | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferManager::_CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Index | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::_CreateUniformBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Uniform | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferManager::_CreateUniformBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Uniform | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::_CreateStorageBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Storage | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferManager::_CreateStorageBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Storage | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::_CreateIndirectBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Indirect | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferManager::_CreateIndirectBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Indirect | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------
    }
}