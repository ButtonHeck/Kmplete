#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/graphics_parameters.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct VulkanGraphicsParameters : public GraphicsParameters
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanGraphicsParameters()
                : GraphicsParameters(GraphicsBackendType::Vulkan)
                  KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
                , vertexAttributeDivisorFeatures(VKUtils::InitVkPhysicalDeviceVertexAttributeDivisorFeatures())
                , lineRasterizationFeatures(VKUtils::InitVkPhysicalDeviceLineRasterizationFeaturesEXT())
                , shaderObjectFeatures(VKUtils::InitVkPhysicalDeviceShaderObjectFeaturesEXT())
                , vertexInputDynamicStateFeatures(VKUtils::InitVkPhysicalDeviceVertexInputDynamicStateFeaturesEXT())
                , dynamicStateFeatures2(VKUtils::InitVkPhysicalDeviceExtendedDynamicState2FeaturesEXT())
                , colorWriteEnableFeatures(VKUtils::InitVkPhysicalDeviceColorWriteEnableFeaturesEXT())
                , depthClipEnableFeatures(VKUtils::InitVkPhysicalDeviceDepthClipEnableFeaturesEXT())
                , dynamicStateFeatures3(VKUtils::InitVkPhysicalDeviceExtendedDynamicState3FeaturesEXT())
                , features({})
                , features13(VKUtils::InitVkPhysicalDeviceVulkan13Features())
                , features2(VKUtils::InitVkPhysicalDeviceFeatures2())
                , maxDescriptorSets(0)
                , descriptorPoolSizes()
            {
                lineRasterizationFeatures.pNext = &vertexAttributeDivisorFeatures;
                shaderObjectFeatures.pNext = &lineRasterizationFeatures;
                vertexInputDynamicStateFeatures.pNext = &shaderObjectFeatures;
                dynamicStateFeatures2.pNext = &vertexInputDynamicStateFeatures;
                colorWriteEnableFeatures.pNext = &dynamicStateFeatures2;
                depthClipEnableFeatures.pNext = &colorWriteEnableFeatures;
                dynamicStateFeatures3.pNext = &depthClipEnableFeatures;
                features13.pNext = &dynamicStateFeatures3;
                features2.pNext = &features13;

                KMP_PROFILE_CONSTRUCTOR_END()
            }

            VkPhysicalDeviceVertexAttributeDivisorFeatures vertexAttributeDivisorFeatures;
            VkPhysicalDeviceLineRasterizationFeaturesEXT lineRasterizationFeatures;
            VkPhysicalDeviceShaderObjectFeaturesEXT shaderObjectFeatures;
            VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT vertexInputDynamicStateFeatures;
            VkPhysicalDeviceExtendedDynamicState2FeaturesEXT dynamicStateFeatures2;
            VkPhysicalDeviceColorWriteEnableFeaturesEXT colorWriteEnableFeatures;
            VkPhysicalDeviceDepthClipEnableFeaturesEXT depthClipEnableFeatures;
            VkPhysicalDeviceExtendedDynamicState3FeaturesEXT dynamicStateFeatures3;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceVulkan13Features features13;
            VkPhysicalDeviceFeatures2 features2;

            UInt32 maxDescriptorSets;
            Vector<VkDescriptorPoolSize> descriptorPoolSizes;
        };
        //--------------------------------------------------------------------------
    }
}