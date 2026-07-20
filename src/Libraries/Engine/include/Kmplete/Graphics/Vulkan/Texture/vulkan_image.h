#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //! Vulkan API image object wrapper
        class KMP_API VulkanImage
        {
            KMP_DISABLE_COPY(VulkanImage)
            KMP_LOG_CLASSNAME(VulkanImage)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanImage(VkDevice device, const VkImageCreateInfo& creationParameters, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties);
            VulkanImage(VulkanImage&& other) noexcept;
            VulkanImage& operator=(VulkanImage&& other) noexcept;
            ~VulkanImage();

            KMP_NODISCARD VkImage GetVkImage() const noexcept;
            KMP_NODISCARD VkDeviceSize GetMemorySize() const noexcept;
            KMP_NODISCARD VkFormat GetVkFormat() const noexcept;
            KMP_NODISCARD VkSampleCountFlagBits GetSamples() const noexcept;

        private:
            void _Initialize(const VkImageCreateInfo& creationParameters, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties);
            void _Finalize();

        private:
            VkDevice _device;
            VkImage _image;
            VkDeviceMemory _imageMemory;
            VkDeviceSize _memorySize;
            VkFormat _format;
            VkSampleCountFlagBits _samples;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanImage>::value);
        static_assert(IsMoveAssignable<VulkanImage>::value);
    }
}