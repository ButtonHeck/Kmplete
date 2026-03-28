#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanRenderer::VulkanRenderer(VkDevice device, VkCommandPool commandPool, const UInt32& currentBufferIndex, const VkExtent2D& currentExtent)
            : _currentBufferIndex(currentBufferIndex)
            , _currentExtent(currentExtent)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _drawCommandBuffers.reserve(NumConcurrentFrames);
            for (size_t i = 0; i < NumConcurrentFrames; i++)
            {
                _drawCommandBuffers.emplace_back(device, commandPool);
            }
        }
        //--------------------------------------------------------------------------

        VulkanRenderer::~VulkanRenderer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _drawCommandBuffers.clear();
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::StartFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            _drawCommandBuffers[_currentBufferIndex].Reset();
            _drawCommandBuffers[_currentBufferIndex].Begin();
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            _drawCommandBuffers[_currentBufferIndex].End();
        }
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer() const noexcept
        {
            return _drawCommandBuffers.at(_currentBufferIndex).GetVkCommandBuffer();
        }
        //--------------------------------------------------------------------------
    }
}