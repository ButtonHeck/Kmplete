#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>

#include <utility>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            KMP_API VkResult CheckResult(VkResult result, const char* message, bool throwException = true);

            KMP_API void InsertImageMemoryBarrier(
                VkCommandBuffer cmdbuffer,
                VkImage image,
                VkAccessFlags srcAccessMask,
                VkAccessFlags dstAccessMask,
                VkImageLayout oldImageLayout,
                VkImageLayout newImageLayout,
                VkPipelineStageFlags srcStageMask,
                VkPipelineStageFlags dstStageMask,
                VkImageSubresourceRange subresourceRange
            );

            //TODO: comments
            struct QueueFamilyIndices
            {
                Optional<UInt32> graphicsFamilyIndex{};
                Optional<UInt32> presentFamilyIndex{};

                inline bool IsValid() const noexcept
                {
                    return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
                }
            };
            //--------------------------------------------------------------------------

            //TODO: comments
            struct SurfaceAndPresentModeProperties
            {
                VkSurfaceCapabilitiesKHR surfaceCapabilities{};
                Vector<VkSurfaceFormatKHR> surfaceFormats{};
                Vector<VkPresentModeKHR> presentModes{};

                inline bool IsValid() const noexcept
                {
                    return !surfaceFormats.empty() && !presentModes.empty();
                }
            };
            //--------------------------------------------------------------------------

            KMP_NODISCARD SurfaceAndPresentModeProperties QuerySurfaceAndPresentModeProperties(VkPhysicalDevice device, VkSurfaceKHR surface);
            KMP_NODISCARD QueueFamilyIndices QueryQueueFamiliesIndices(VkPhysicalDevice device, VkSurfaceKHR surface);
            KMP_NODISCARD bool QueryDeviceExtensionSupport(VkPhysicalDevice device, const Vector<const char*>& enabledExtensions);
            KMP_NODISCARD std::pair<bool, std::pair<QueueFamilyIndices, SurfaceAndPresentModeProperties>> IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const Vector<const char*>& enabledExtensions);

            KMP_NODISCARD VkCommandBuffer StartSingleTimeCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool);
            void EndSingleTimeCommandBuffer(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);


            // TODO: comments
            struct ShaderCreateInfo
            {
                Filepath filepath;
                VkShaderStageFlagBits stage;
                const char* entryPointName;
            };
            //--------------------------------------------------------------------------
        }
    }
}