#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer_manager.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_buffer_creator_delegate.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBufferManager::VulkanBufferManager(VkDevice device, const VulkanBufferCreatorDelegate& bufferCreatorDelegate)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _bufferCreatorDelegate(bufferCreatorDelegate)
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
            return _bufferCreatorDelegate.CreateBuffer(parameters);
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
                    _perFrameBuffers[bufferSid][i].reset(_bufferCreatorDelegate.CreateBufferPtr(parameters));
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

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _bufferCreatorDelegate.CreateBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanVertexBuffer VulkanBufferManager::CreateVertexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _bufferCreatorDelegate.CreateVertexBuffer(parameters);
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
                    _perFrameVertexBuffers[bufferSid][i].reset(_bufferCreatorDelegate.CreateVertexBufferPtr(parameters));
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

                const auto [iterator, hasEmplaced] = _vertexBuffers.emplace(bufferSid, _bufferCreatorDelegate.CreateVertexBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateIndexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _bufferCreatorDelegate.CreateIndexBuffer(parameters);
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
                    _perFrameBuffers[bufferSid][i].reset(_bufferCreatorDelegate.CreateIndexBufferPtr(parameters));
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

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _bufferCreatorDelegate.CreateIndexBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateUniformBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _bufferCreatorDelegate.CreateUniformBuffer(parameters);
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
                    _perFrameBuffers[bufferSid][i].reset(_bufferCreatorDelegate.CreateUniformBufferPtr(parameters));
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

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _bufferCreatorDelegate.CreateUniformBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateStorageBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _bufferCreatorDelegate.CreateStorageBuffer(parameters);
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
                    _perFrameBuffers[bufferSid][i].reset(_bufferCreatorDelegate.CreateStorageBufferPtr(parameters));
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

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _bufferCreatorDelegate.CreateStorageBufferPtr(parameters));
                return hasEmplaced;
            }
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferManager::CreateIndirectBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return _bufferCreatorDelegate.CreateIndirectBuffer(parameters);
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
                    _perFrameBuffers[bufferSid][i].reset(_bufferCreatorDelegate.CreateIndirectBufferPtr(parameters));
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

                const auto [iterator, hasEmplaced] = _buffers.emplace(bufferSid, _bufferCreatorDelegate.CreateIndirectBufferPtr(parameters));
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
    }
}