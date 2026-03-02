#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Utils/helper_structs.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanTexture::VulkanTexture(VkDevice device, VkQueue graphicsQueue, const Image& image, const VulkanImageCreatorDelegate& imageCreator, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkCommandPool commandPool)
            : _logicalDevice(device)
            , _image(nullptr)
            , _imageView(VK_NULL_HANDLE)
            , _sampler(VK_NULL_HANDLE)
            , _buffer(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            VulkanUtils::ImageParameters creationParameters = {
                .width = UInt32(image.GetWidth()),
                .height = UInt32(image.GetHeight()),
                .mipLevels = 1,
                .numSamples = VK_SAMPLE_COUNT_1_BIT,
                .format = VK_FORMAT_R8G8B8A8_UNORM,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };
            _image.reset(imageCreator.CreateImagePtr(creationParameters));
            auto vulkanImage = _image->GetVkImage();

            _imageView = imageCreator.CreateImageView(vulkanImage, creationParameters.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

            VulkanUtils::SamplerParameters samplerParameters = {
                .magnificationFilter = VK_FILTER_LINEAR,
                .minificationFilter = VK_FILTER_LINEAR,
                .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                .minLod = -1000,
                .maxLod = 1000,
                .maxAnisotropy = 1.0f
            };
            _sampler = imageCreator.CreateSampler(samplerParameters);

            _InitializeBuffer(memoryTypeDelegate, image);

            auto commandBufferAllocateInfo = VulkanUtils::InitVkCommandBufferAllocateInfo();
            commandBufferAllocateInfo.commandPool = commandPool;
            commandBufferAllocateInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            const auto result = vkAllocateCommandBuffers(_logicalDevice, &commandBufferAllocateInfo, &commandBuffer);

            auto commandBufferBeginInfo = VulkanUtils::InitVkCommandBufferBeginInfo();
            commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

            VulkanUtils::InsertImageMemoryBarrier(
                commandBuffer,
                vulkanImage,
                VK_ACCESS_NONE,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_PIPELINE_STAGE_HOST_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            );

            VkBufferImageCopy region{};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent.width = UInt32(image.GetWidth());
            region.imageExtent.height = UInt32(image.GetHeight());
            region.imageExtent.depth = 1;
            vkCmdCopyBufferToImage(commandBuffer, _buffer->GetVkBuffer(), vulkanImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VulkanUtils::InsertImageMemoryBarrier(
                commandBuffer,
                vulkanImage,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_ACCESS_SHADER_READ_BIT,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            );

            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            vkEndCommandBuffer(commandBuffer);
            vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
            vkDeviceWaitIdle(_logicalDevice);
        }
        //--------------------------------------------------------------------------

        VulkanTexture::~VulkanTexture()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _buffer.reset();

            vkDestroySampler(_logicalDevice, _sampler, nullptr);
            vkDestroyImageView(_logicalDevice, _imageView, nullptr);

            _image.reset();
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanTexture::GetVkImageView() const noexcept
        {
            return _imageView;
        }
        //--------------------------------------------------------------------------

        VkSampler VulkanTexture::GetVkSampler() const noexcept
        {
            return _sampler;
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_InitializeBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Image& image)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _buffer.reset(new VulkanBuffer(memoryTypeDelegate, _logicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, image.GetDataSize()));
            if (!_buffer)
            {
                KMP_LOG_ERROR("failed to create texture buffer");
                throw std::runtime_error("VulkanTexture: failed to create texture buffer");
            }

            auto result = _buffer->Map();
            VulkanUtils::CheckResult(result, "VulkanTexture: failed to map texture buffer");

            _buffer->CopyToMappedMemory(image.GetPixels(), image.GetDataSize());

            result = _buffer->Flush();
            VulkanUtils::CheckResult(result, "VulkanTexture: failed to flush texture buffer");

            _buffer->Unmap();
        }
        //--------------------------------------------------------------------------
    }
}