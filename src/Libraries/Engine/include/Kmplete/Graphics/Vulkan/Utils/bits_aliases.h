#pragma once

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VKBits
        {
            static constexpr auto VK_BufferUsage_TransferSrc = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            static constexpr auto VK_BufferUsage_TransferDst = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            static constexpr auto VK_BufferUsage_UniformTexel = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_StorageTexel = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_Uniform = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_Storage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_Index = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_Vertex = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_Indirect = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
            static constexpr auto VK_BufferUsage_ShaderDeviceAddress = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

            static constexpr auto VK_Image_1D = VK_IMAGE_TYPE_1D;
            static constexpr auto VK_Image_2D = VK_IMAGE_TYPE_2D;
            static constexpr auto VK_Image_3D = VK_IMAGE_TYPE_3D;

            static constexpr auto VK_ImageView_1D = VK_IMAGE_VIEW_TYPE_1D;
            static constexpr auto VK_ImageView_2D = VK_IMAGE_VIEW_TYPE_2D;
            static constexpr auto VK_ImageView_3D = VK_IMAGE_VIEW_TYPE_3D;
            static constexpr auto VK_ImageView_Cube = VK_IMAGE_VIEW_TYPE_CUBE;
            static constexpr auto VK_ImageView_1DArray = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
            static constexpr auto VK_ImageView_2DArray = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            static constexpr auto VK_ImageView_CubeArray = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

            static constexpr auto VK_ImageAspect_Color = VK_IMAGE_ASPECT_COLOR_BIT;
            static constexpr auto VK_ImageAspect_Depth = VK_IMAGE_ASPECT_DEPTH_BIT;
            static constexpr auto VK_ImageAspect_Stencil = VK_IMAGE_ASPECT_STENCIL_BIT;
            static constexpr auto VK_ImageAspect_Metadata = VK_IMAGE_ASPECT_METADATA_BIT;
            static constexpr auto VK_ImageAspect_None = VK_IMAGE_ASPECT_NONE;
            static constexpr auto VK_ImageAspect_DepthStencil = VK_ImageAspect_Depth | VK_ImageAspect_Stencil;

            static constexpr auto VK_ImageTiling_Optimal = VK_IMAGE_TILING_OPTIMAL;
            static constexpr auto VK_ImageTiling_Linear = VK_IMAGE_TILING_LINEAR;

            static constexpr auto VK_ImageLayout_Undefined = VK_IMAGE_LAYOUT_UNDEFINED;
            static constexpr auto VK_ImageLayout_General = VK_IMAGE_LAYOUT_GENERAL;
            static constexpr auto VK_ImageLayout_ColorAttachmentOptimal = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            static constexpr auto VK_ImageLayout_DepthStencilAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            static constexpr auto VK_ImageLayout_DepthStencilReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            static constexpr auto VK_ImageLayout_ShaderReadOnlyOptimal = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            static constexpr auto VK_ImageLayout_TransferSrcOptimal = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            static constexpr auto VK_ImageLayout_TransferDstOptimal = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            static constexpr auto VK_ImageLayout_Preinitialized = VK_IMAGE_LAYOUT_PREINITIALIZED;
            static constexpr auto VK_ImageLayout_DepthReadOnlyStencilAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
            static constexpr auto VK_ImageLayout_DepthAttachmentStencilReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
            static constexpr auto VK_ImageLayout_DepthAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
            static constexpr auto VK_ImageLayout_DepthReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
            static constexpr auto VK_ImageLayout_StencilAttachmentOptimal = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
            static constexpr auto VK_ImageLayout_StencilReadOnlyOptimal = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
            static constexpr auto VK_ImageLayout_ReadOnlyOptimal = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
            static constexpr auto VK_ImageLayout_AttachmentOptimal = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
            static constexpr auto VK_ImageLayout_RenderingLocalRead = VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ;
            static constexpr auto VK_ImageLayout_PresentKHR = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            static constexpr auto VK_ImageUsage_TransferSrc = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            static constexpr auto VK_ImageUsage_TransferDst = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            static constexpr auto VK_ImageUsage_Sampled = VK_IMAGE_USAGE_SAMPLED_BIT;
            static constexpr auto VK_ImageUsage_Storage = VK_IMAGE_USAGE_STORAGE_BIT;
            static constexpr auto VK_ImageUsage_ColorAttachment = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            static constexpr auto VK_ImageUsage_DepthStencilAttachment = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            static constexpr auto VK_ImageUsage_TransientAttachment = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
            static constexpr auto VK_ImageUsage_InputAttachment = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
            static constexpr auto VK_ImageUsage_HostTransfer = VK_IMAGE_USAGE_HOST_TRANSFER_BIT;
            static constexpr auto VK_ImageUsage_VideoDecodeDst = VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR;
            static constexpr auto VK_ImageUsage_VideoDecodeSrc = VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
            static constexpr auto VK_ImageUsage_VideoDecodeDPB = VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR;
            static constexpr auto VK_ImageUsage_FragmentDensityMap = VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;
            static constexpr auto VK_ImageUsage_FragmentShadingRateAttachment = VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
            static constexpr auto VK_ImageUsage_VideoEncodeDst = VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
            static constexpr auto VK_ImageUsage_VideoEncodeSrc = VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
            static constexpr auto VK_ImageUsage_VideoEncodeDPB = VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR;
            static constexpr auto VK_ImageUsage_AttachmentFeedbackLoop = VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;
            static constexpr auto VK_ImageUsage_TransferSrcAndDst = VK_ImageUsage_TransferSrc | VK_ImageUsage_TransferDst;

            static constexpr auto VK_Filter_Nearest = VK_FILTER_NEAREST;
            static constexpr auto VK_Filter_Linear = VK_FILTER_LINEAR;

            static constexpr auto VK_SamplerMipmap_Nearest = VK_SAMPLER_MIPMAP_MODE_NEAREST;
            static constexpr auto VK_SamplerMipmap_Linear = VK_SAMPLER_MIPMAP_MODE_LINEAR;

            static constexpr auto VK_SamplerAddress_Repeat = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            static constexpr auto VK_SamplerAddress_MirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            static constexpr auto VK_SamplerAddress_ClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            static constexpr auto VK_SamplerAddress_ClampToBorder = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            static constexpr auto VK_SamplerAddress_MirrorClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

            static constexpr auto VK_FormatFeature_SampledImageFilterLinear = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
            static constexpr auto VK_FormatFeature_DepthStencilAttachment = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

            static constexpr auto VK_Memory_DeviceLocal = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            static constexpr auto VK_Memory_HostVisible = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            static constexpr auto VK_Memory_HostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            static constexpr auto VK_Memory_HostCached = VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
            static constexpr auto VK_Memory_LazilyAllocated = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
            static constexpr auto VK_Memory_Protected = VK_MEMORY_PROPERTY_PROTECTED_BIT;

            static constexpr auto VK_MemoryAllocate_DeviceMask = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT;
            static constexpr auto VK_MemoryAllocate_DeviceAddress = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

            static constexpr auto VK_Queue_Graphics = VK_QUEUE_GRAPHICS_BIT;
            static constexpr auto VK_Queue_Compute = VK_QUEUE_COMPUTE_BIT;
            static constexpr auto VK_Queue_Transfer = VK_QUEUE_TRANSFER_BIT;
            static constexpr auto VK_Queue_SparseBinding = VK_QUEUE_SPARSE_BINDING_BIT;

            static constexpr auto VK_CommandBuffer_Primary = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            static constexpr auto VK_CommandBuffer_Secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

            static constexpr auto VK_FenceCreate_Signaled = VK_FENCE_CREATE_SIGNALED_BIT;

            static constexpr auto VK_DescriptorPoolCreate_FreeDescriptorSet = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            static constexpr auto VK_DescriptorPoolCreate_UpdateAfterBind = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

            static constexpr auto VK_Color_R = VK_COLOR_COMPONENT_R_BIT;
            static constexpr auto VK_Color_G = VK_COLOR_COMPONENT_G_BIT;
            static constexpr auto VK_Color_B = VK_COLOR_COMPONENT_B_BIT;
            static constexpr auto VK_Color_A = VK_COLOR_COMPONENT_A_BIT;
            static constexpr auto VK_Color_RGBA = VK_Color_R | VK_Color_G | VK_Color_B | VK_Color_A;

            static constexpr auto VK_ColorSpace_SRGB_Nonlinear = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

            static constexpr auto VK_BlendFactor_Zero = VK_BLEND_FACTOR_ZERO;
            static constexpr auto VK_BlendFactor_One = VK_BLEND_FACTOR_ONE;
            static constexpr auto VK_BlendFactor_SrcColor = VK_BLEND_FACTOR_SRC_COLOR;
            static constexpr auto VK_BlendFactor_OneMinusSrcColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            static constexpr auto VK_BlendFactor_DstColor = VK_BLEND_FACTOR_DST_COLOR;
            static constexpr auto VK_BlendFactor_OneMonusDstColor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            static constexpr auto VK_BlendFactor_SrcAlpha = VK_BLEND_FACTOR_SRC_ALPHA;
            static constexpr auto VK_BlendFactor_OneMinusSrcAlpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            static constexpr auto VK_BlendFactor_DstAlpha = VK_BLEND_FACTOR_DST_ALPHA;
            static constexpr auto VK_BlendFactor_OneMinusDstAlpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            static constexpr auto VK_BlendFactor_ConstColor = VK_BLEND_FACTOR_CONSTANT_COLOR;
            static constexpr auto VK_BlendFactor_OneMinusConstColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
            static constexpr auto VK_BlendFactor_ConstAlpha = VK_BLEND_FACTOR_CONSTANT_ALPHA;
            static constexpr auto VK_BlendFactor_OneMinusConstAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
            static constexpr auto VK_BlendFactor_SrcAlphaSaturate = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
            static constexpr auto VK_BlendFactor_Src1Color = VK_BLEND_FACTOR_SRC1_COLOR;
            static constexpr auto VK_BlendFactor_OneMinusSrc1Color = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
            static constexpr auto VK_BlendFactor_Src1Alpha = VK_BLEND_FACTOR_SRC1_ALPHA;
            static constexpr auto VK_BlendFactor_OneMinusSrc1Alpha = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;

            static constexpr auto VK_Blend_Add = VK_BLEND_OP_ADD;
            static constexpr auto VK_Blend_Subtract = VK_BLEND_OP_SUBTRACT;
            static constexpr auto VK_Blend_ReverseSubtract = VK_BLEND_OP_REVERSE_SUBTRACT;
            static constexpr auto VK_Blend_Min = VK_BLEND_OP_MIN;
            static constexpr auto VK_Blend_Max = VK_BLEND_OP_MAX;

            static constexpr auto VK_Stencil_Keep = VK_STENCIL_OP_KEEP;
            static constexpr auto VK_Stencil_Zero = VK_STENCIL_OP_ZERO;
            static constexpr auto VK_Stencil_Replace = VK_STENCIL_OP_REPLACE;
            static constexpr auto VK_Stencil_IncrementAndClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
            static constexpr auto VK_Stencil_DecrementAndClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
            static constexpr auto VK_Stencil_Invert = VK_STENCIL_OP_INVERT;
            static constexpr auto VK_Stencil_IncrementAndWrap = VK_STENCIL_OP_INCREMENT_AND_WRAP;
            static constexpr auto VK_Stencil_DecrementAndWrap = VK_STENCIL_OP_DECREMENT_AND_WRAP;

            static constexpr auto VK_Compare_Never = VK_COMPARE_OP_NEVER;
            static constexpr auto VK_Compare_Less = VK_COMPARE_OP_LESS;
            static constexpr auto VK_Compare_Equal = VK_COMPARE_OP_EQUAL;
            static constexpr auto VK_Compare_LessOrEqual = VK_COMPARE_OP_LESS_OR_EQUAL;
            static constexpr auto VK_Compare_Greater = VK_COMPARE_OP_GREATER;
            static constexpr auto VK_Compare_NotEqual = VK_COMPARE_OP_NOT_EQUAL;
            static constexpr auto VK_Compare_GreaterOrEqual = VK_COMPARE_OP_GREATER_OR_EQUAL;
            static constexpr auto VK_Compare_Always = VK_COMPARE_OP_ALWAYS;

            static constexpr auto VK_Sharing_Exclusive = VK_SHARING_MODE_EXCLUSIVE;
            static constexpr auto VK_Sharing_Concurrent = VK_SHARING_MODE_CONCURRENT;

            static constexpr auto VK_AttachmentLoad_Load = VK_ATTACHMENT_LOAD_OP_LOAD;
            static constexpr auto VK_AttachmentLoad_Clear = VK_ATTACHMENT_LOAD_OP_CLEAR;
            static constexpr auto VK_AttachmentLoad_DontCare = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            static constexpr auto VK_AttachmentLoad_None = VK_ATTACHMENT_LOAD_OP_NONE;

            static constexpr auto VK_AttachmentStore_Store = VK_ATTACHMENT_STORE_OP_STORE;
            static constexpr auto VK_AttachmentStore_DontCare = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            static constexpr auto VK_AttachmentStore_None = VK_ATTACHMENT_STORE_OP_NONE;

            static constexpr auto VK_CommandPoolCreate_Transient = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            static constexpr auto VK_CommandPoolCreate_ResetCommandBuffer = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            static constexpr auto VK_CommandPoolCreate_Protected = VK_COMMAND_POOL_CREATE_PROTECTED_BIT;

            static constexpr auto VK_CommandBufferUsage_OneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            static constexpr auto VK_CommandBufferUsage_RenderPassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
            static constexpr auto VK_CommandBufferUsage_SimultaneousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

            static constexpr auto VK_SampleCount_1 = VK_SAMPLE_COUNT_1_BIT;
            static constexpr auto VK_SampleCount_2 = VK_SAMPLE_COUNT_2_BIT;
            static constexpr auto VK_SampleCount_4 = VK_SAMPLE_COUNT_4_BIT;
            static constexpr auto VK_SampleCount_8 = VK_SAMPLE_COUNT_8_BIT;
            static constexpr auto VK_SampleCount_16 = VK_SAMPLE_COUNT_16_BIT;
            static constexpr auto VK_SampleCount_32 = VK_SAMPLE_COUNT_32_BIT;
            static constexpr auto VK_SampleCount_64 = VK_SAMPLE_COUNT_64_BIT;

            static constexpr auto VK_DescriptorType_Sampler = VK_DESCRIPTOR_TYPE_SAMPLER;
            static constexpr auto VK_DescriptorType_CombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            static constexpr auto VK_DescriptorType_SampledImage = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            static constexpr auto VK_DescriptorType_StorageImage = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            static constexpr auto VK_DescriptorType_UniformTexelBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
            static constexpr auto VK_DescriptorType_StorageTexelBuffer = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
            static constexpr auto VK_DescriptorType_UniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            static constexpr auto VK_DescriptorType_StorageBuffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            static constexpr auto VK_DescriptorType_UniformBufferDynamic = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            static constexpr auto VK_DescriptorType_StorageBufferDynamic = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            static constexpr auto VK_DescriptorType_InputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            static constexpr auto VK_DescriptorType_InlineUniformBlock = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;

            static constexpr auto VK_Format_Undefined = VK_FORMAT_UNDEFINED;
            static constexpr auto VK_Format_RG4_UNorm_Pack8 = VK_FORMAT_R4G4_UNORM_PACK8;
            static constexpr auto VK_Format_RGBA4_UNorm_Pack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
            static constexpr auto VK_Format_BGRA4_UNorm_Pack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16;
            static constexpr auto VK_Format_R5G6B5_UNorm_Pack16 = VK_FORMAT_R5G6B5_UNORM_PACK16;
            static constexpr auto VK_Format_B5G6R5_UNorm_Pack16 = VK_FORMAT_B5G6R5_UNORM_PACK16;
            static constexpr auto VK_Format_RGB5A1_UNorm_Pack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
            static constexpr auto VK_Format_BGR5A1_UNorm_Pack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
            static constexpr auto VK_Format_A1RGB5_UNorm_Pack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16;
            static constexpr auto VK_Format_R8_UNorm = VK_FORMAT_R8_UNORM;
            static constexpr auto VK_Format_R8_SNorm = VK_FORMAT_R8_SNORM;
            static constexpr auto VK_Format_R8_UScaled = VK_FORMAT_R8_USCALED;
            static constexpr auto VK_Format_R8_SScaled = VK_FORMAT_R8_SSCALED;
            static constexpr auto VK_Format_R8_UInt = VK_FORMAT_R8_UINT;
            static constexpr auto VK_Format_R8_SInt = VK_FORMAT_R8_SINT;
            static constexpr auto VK_Format_R8_SRGB = VK_FORMAT_R8_SRGB;
            static constexpr auto VK_Format_RG8_UNorm = VK_FORMAT_R8G8_UNORM;
            static constexpr auto VK_Format_RG8_SNorm = VK_FORMAT_R8G8_SNORM;
            static constexpr auto VK_Format_RG8_UScaled = VK_FORMAT_R8G8_USCALED;
            static constexpr auto VK_Format_RG8_SScaled = VK_FORMAT_R8G8_SSCALED;
            static constexpr auto VK_Format_RG8_UInt = VK_FORMAT_R8G8_UINT;
            static constexpr auto VK_Format_RG8_SInt = VK_FORMAT_R8G8_SINT;
            static constexpr auto VK_Format_RG8_SRGB = VK_FORMAT_R8G8_SRGB;
            static constexpr auto VK_Format_RGB8_UNorm = VK_FORMAT_R8G8B8_UNORM;
            static constexpr auto VK_Format_RGB8_SNorm = VK_FORMAT_R8G8B8_SNORM;
            static constexpr auto VK_Format_RGB8_UScaled = VK_FORMAT_R8G8B8_USCALED;
            static constexpr auto VK_Format_RGB8_SScaled = VK_FORMAT_R8G8B8_SSCALED;
            static constexpr auto VK_Format_RGB8_UInt = VK_FORMAT_R8G8B8_UINT;
            static constexpr auto VK_Format_RGB8_SInt = VK_FORMAT_R8G8B8_SINT;
            static constexpr auto VK_Format_RGB8_SRGB = VK_FORMAT_R8G8B8_SRGB;
            static constexpr auto VK_Format_BGR8_UNorm = VK_FORMAT_B8G8R8_UNORM;
            static constexpr auto VK_Format_BGR8_SNorm = VK_FORMAT_B8G8R8_SNORM;
            static constexpr auto VK_Format_BGR8_UScaled = VK_FORMAT_B8G8R8_USCALED;
            static constexpr auto VK_Format_BGR8_SScaled = VK_FORMAT_B8G8R8_SSCALED;
            static constexpr auto VK_Format_BGR8_UInt = VK_FORMAT_B8G8R8_UINT;
            static constexpr auto VK_Format_BGR8_SInt = VK_FORMAT_B8G8R8_SINT;
            static constexpr auto VK_Format_BGR8_SRGB = VK_FORMAT_B8G8R8_SRGB;
            static constexpr auto VK_Format_RGBA8_UNorm = VK_FORMAT_R8G8B8A8_UNORM;
            static constexpr auto VK_Format_RGBA8_SNorm = VK_FORMAT_R8G8B8A8_SNORM;
            static constexpr auto VK_Format_RGBA8_UScaled = VK_FORMAT_R8G8B8A8_USCALED;
            static constexpr auto VK_Format_RGBA8_SScaled = VK_FORMAT_R8G8B8A8_SSCALED;
            static constexpr auto VK_Format_RGBA8_UInt = VK_FORMAT_R8G8B8A8_UINT;
            static constexpr auto VK_Format_RGBA8_SInt = VK_FORMAT_R8G8B8A8_SINT;
            static constexpr auto VK_Format_RGBA8_SRGB = VK_FORMAT_R8G8B8A8_SRGB;
            static constexpr auto VK_Format_BGRA8_UNorm = VK_FORMAT_B8G8R8A8_UNORM;
            static constexpr auto VK_Format_BGRA8_SNorm = VK_FORMAT_B8G8R8A8_SNORM;
            static constexpr auto VK_Format_BGRA8_UScaled = VK_FORMAT_B8G8R8A8_USCALED;
            static constexpr auto VK_Format_BGRA8_SScaled = VK_FORMAT_B8G8R8A8_SSCALED;
            static constexpr auto VK_Format_BGRA8_UInt = VK_FORMAT_B8G8R8A8_UINT;
            static constexpr auto VK_Format_BGRA8_SInt = VK_FORMAT_B8G8R8A8_SINT;
            static constexpr auto VK_Format_BGRA8_SRGB = VK_FORMAT_B8G8R8A8_SRGB;
            static constexpr auto VK_Format_ABGR8_UNorm_Pack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32;
            static constexpr auto VK_Format_ABGR8_SNorm_Pack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32;
            static constexpr auto VK_Format_ABGR8_UScaled_Pack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32;
            static constexpr auto VK_Format_ABGR8_SScaled_Pack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
            static constexpr auto VK_Format_ABGR8_UInt_Pack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32;
            static constexpr auto VK_Format_ABGR8_SInt_Pack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32;
            static constexpr auto VK_Format_ABGR8_SRGB_Pack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32;
            static constexpr auto VK_Format_A2RGB10_UNorm_Pack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            static constexpr auto VK_Format_A2RGB10_SNorm_Pack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32;
            static constexpr auto VK_Format_A2RGB10_UScaled_Pack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32;
            static constexpr auto VK_Format_A2RGB10_SScaled_Pack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
            static constexpr auto VK_Format_A2RGB10_UInt_Pack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32;
            static constexpr auto VK_Format_A2RGB10_SInt_Pack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32;
            static constexpr auto VK_Format_A2BGR10_UNorm_Pack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
            static constexpr auto VK_Format_A2BGR10_SNorm_Pack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32;
            static constexpr auto VK_Format_A2BGR10_UScaled_Pack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32;
            static constexpr auto VK_Format_A2BGR10_SScaled_Pack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
            static constexpr auto VK_Format_A2BGR10_UInt_Pack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32;
            static constexpr auto VK_Format_A2BGR10_SInt_Pack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32;
            static constexpr auto VK_Format_R16_UNorm = VK_FORMAT_R16_UNORM;
            static constexpr auto VK_Format_R16_SNorm = VK_FORMAT_R16_SNORM;
            static constexpr auto VK_Format_R16_UScaled = VK_FORMAT_R16_USCALED;
            static constexpr auto VK_Format_R16_SScaled = VK_FORMAT_R16_SSCALED;
            static constexpr auto VK_Format_R16_UInt = VK_FORMAT_R16_UINT;
            static constexpr auto VK_Format_R16_SInt = VK_FORMAT_R16_SINT;
            static constexpr auto VK_Format_R16_SFloat = VK_FORMAT_R16_SFLOAT;
            static constexpr auto VK_Format_RG16_UNorm = VK_FORMAT_R16G16_UNORM;
            static constexpr auto VK_Format_RG16_SNorm = VK_FORMAT_R16G16_SNORM;
            static constexpr auto VK_Format_RG16_UScaled = VK_FORMAT_R16G16_USCALED;
            static constexpr auto VK_Format_RG16_SScaled = VK_FORMAT_R16G16_SSCALED;
            static constexpr auto VK_Format_RG16_UInt = VK_FORMAT_R16G16_UINT;
            static constexpr auto VK_Format_RG16_SInt = VK_FORMAT_R16G16_SINT;
            static constexpr auto VK_Format_RG16_SFloat = VK_FORMAT_R16G16_SFLOAT;
            static constexpr auto VK_Format_RGB16_UNorm = VK_FORMAT_R16G16B16_UNORM;
            static constexpr auto VK_Format_RGB16_SNorm = VK_FORMAT_R16G16B16_SNORM;
            static constexpr auto VK_Format_RGB16_UScaled = VK_FORMAT_R16G16B16_USCALED;
            static constexpr auto VK_Format_RGB16_SScaled = VK_FORMAT_R16G16B16_SSCALED;
            static constexpr auto VK_Format_RGB16_UInt = VK_FORMAT_R16G16B16_UINT;
            static constexpr auto VK_Format_RGB16_SInt = VK_FORMAT_R16G16B16_SINT;
            static constexpr auto VK_Format_RGB16_SFloat = VK_FORMAT_R16G16B16_SFLOAT;
            static constexpr auto VK_Format_RGBA16_UNorm = VK_FORMAT_R16G16B16A16_UNORM;
            static constexpr auto VK_Format_RGBA16_SNorm = VK_FORMAT_R16G16B16A16_SNORM;
            static constexpr auto VK_Format_RGBA16_UScaled = VK_FORMAT_R16G16B16A16_USCALED;
            static constexpr auto VK_Format_RGBA16_SScaled = VK_FORMAT_R16G16B16A16_SSCALED;
            static constexpr auto VK_Format_RGBA16_UInt = VK_FORMAT_R16G16B16A16_UINT;
            static constexpr auto VK_Format_RGBA16_SInt = VK_FORMAT_R16G16B16A16_SINT;
            static constexpr auto VK_Format_RGBA16_SFloat = VK_FORMAT_R16G16B16A16_SFLOAT;
            static constexpr auto VK_Format_R32_UInt = VK_FORMAT_R32_UINT;
            static constexpr auto VK_Format_R32_SInt = VK_FORMAT_R32_SINT;
            static constexpr auto VK_Format_R32_SFloat = VK_FORMAT_R32_SFLOAT;
            static constexpr auto VK_Format_RG32_UInt = VK_FORMAT_R32G32_UINT;
            static constexpr auto VK_Format_RG32_SInt = VK_FORMAT_R32G32_SINT;
            static constexpr auto VK_Format_RG32_SFloat = VK_FORMAT_R32G32_SFLOAT;
            static constexpr auto VK_Format_RGB32_UInt = VK_FORMAT_R32G32B32_UINT;
            static constexpr auto VK_Format_RGB32_SInt = VK_FORMAT_R32G32B32_SINT;
            static constexpr auto VK_Format_RGB32_SFloat = VK_FORMAT_R32G32B32_SFLOAT;
            static constexpr auto VK_Format_RGBA32_UInt = VK_FORMAT_R32G32B32A32_UINT;
            static constexpr auto VK_Format_RGBA32_SInt = VK_FORMAT_R32G32B32A32_SINT;
            static constexpr auto VK_Format_RGBA32_SFloat = VK_FORMAT_R32G32B32A32_SFLOAT;
            static constexpr auto VK_Format_R64_UInt = VK_FORMAT_R64_UINT;
            static constexpr auto VK_Format_R64_SInt = VK_FORMAT_R64_SINT;
            static constexpr auto VK_Format_R64_SFloat = VK_FORMAT_R64_SFLOAT;
            static constexpr auto VK_Format_RG64_UInt = VK_FORMAT_R64G64_UINT;
            static constexpr auto VK_Format_RG64_SInt = VK_FORMAT_R64G64_SINT;
            static constexpr auto VK_Format_RG64_SFloat = VK_FORMAT_R64G64_SFLOAT;
            static constexpr auto VK_Format_RGB64_UInt = VK_FORMAT_R64G64B64_UINT;
            static constexpr auto VK_Format_RGB64_SInt = VK_FORMAT_R64G64B64_SINT;
            static constexpr auto VK_Format_RGB64_SFloat = VK_FORMAT_R64G64B64_SFLOAT;
            static constexpr auto VK_Format_RGBA64_UInt = VK_FORMAT_R64G64B64A64_UINT;
            static constexpr auto VK_Format_RGBA64_SInt = VK_FORMAT_R64G64B64A64_SINT;
            static constexpr auto VK_Format_RGBA64_SFloat = VK_FORMAT_R64G64B64A64_SFLOAT;
            static constexpr auto VK_Format_B10GR11_UFloat_Pack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
            static constexpr auto VK_Format_E5BGR9_UFloat_Pack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
            static constexpr auto VK_Format_D16_UNorm = VK_FORMAT_D16_UNORM;
            static constexpr auto VK_Format_X8D24_UNorm_Pack32 = VK_FORMAT_X8_D24_UNORM_PACK32;
            static constexpr auto VK_Format_D32_SFloat = VK_FORMAT_D32_SFLOAT;
            static constexpr auto VK_Format_S8_UInt = VK_FORMAT_S8_UINT;
            static constexpr auto VK_Format_D16_UNorm_S8_UInt = VK_FORMAT_D16_UNORM_S8_UINT;
            static constexpr auto VK_Format_D24_UNorm_S8_UInt = VK_FORMAT_D24_UNORM_S8_UINT;
            static constexpr auto VK_Format_D32_SFloat_S8_UInt = VK_FORMAT_D32_SFLOAT_S8_UINT;

            static constexpr auto VK_DebugUtilsMessageSeverity_Verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
            static constexpr auto VK_DebugUtilsMessageSeverity_Info = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
            static constexpr auto VK_DebugUtilsMessageSeverity_Warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
            static constexpr auto VK_DebugUtilsMessageSeverity_Error = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

            static constexpr auto VK_DebugUtilsMessageType_General = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
            static constexpr auto VK_DebugUtilsMessageType_Validation = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
            static constexpr auto VK_DebugUtilsMessageType_Performance = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            static constexpr auto VK_DebugUtilsMessageType_DeviceAddressBinding = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;

            static constexpr auto VK_Dynamic_Viewport = VK_DYNAMIC_STATE_VIEWPORT;
            static constexpr auto VK_Dynamic_Scissor = VK_DYNAMIC_STATE_SCISSOR;
            static constexpr auto VK_Dynamic_LineWidth = VK_DYNAMIC_STATE_LINE_WIDTH;
            static constexpr auto VK_Dynamic_DepthBias = VK_DYNAMIC_STATE_DEPTH_BIAS;
            static constexpr auto VK_Dynamic_BlendConstants = VK_DYNAMIC_STATE_BLEND_CONSTANTS;
            static constexpr auto VK_Dynamic_DepthBounds = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
            static constexpr auto VK_Dynamic_StencilCompareMask = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
            static constexpr auto VK_Dynamic_StencilWriteMask = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
            static constexpr auto VK_Dynamic_StencilReference = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
            static constexpr auto VK_Dynamic_CullMode = VK_DYNAMIC_STATE_CULL_MODE;
            static constexpr auto VK_Dynamic_FrontFace = VK_DYNAMIC_STATE_FRONT_FACE;
            static constexpr auto VK_Dynamic_PrimitiveTopology = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY;
            static constexpr auto VK_Dynamic_ViewportWithCount = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT;
            static constexpr auto VK_Dynamic_ScissorWithCount = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT;
            static constexpr auto VK_Dynamic_VertexInputBindingStride = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE;
            static constexpr auto VK_Dynamic_DepthTestEnable = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE;
            static constexpr auto VK_Dynamic_DepthWriteEnable = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE;
            static constexpr auto VK_Dynamic_DepthCompareOp = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP;
            static constexpr auto VK_Dynamic_DepthBoundsTestEnable = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE;
            static constexpr auto VK_Dynamic_StencilTestEnable = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE;
            static constexpr auto VK_Dynamic_StencilOp = VK_DYNAMIC_STATE_STENCIL_OP;
            static constexpr auto VK_Dynamic_RasterizerDiscardEnable = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE;
            static constexpr auto VK_Dynamic_DepthBiasEnable = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE;
            static constexpr auto VK_Dynamic_PrimitiveRestartEnable = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE;
            static constexpr auto VK_Dynamic_LineStipple = VK_DYNAMIC_STATE_LINE_STIPPLE;
            static constexpr auto VK_Dynamic_DiscardRectangle = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT;
            static constexpr auto VK_Dynamic_DiscardRectangleEnable = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT;
            static constexpr auto VK_Dynamic_DiscardRectangleMode = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_MODE_EXT;
            static constexpr auto VK_Dynamic_SampleLocations = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT;
            static constexpr auto VK_Dynamic_FragmentShadingRate = VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR;
            static constexpr auto VK_Dynamic_VertexInputBit = VK_DYNAMIC_STATE_VERTEX_INPUT_EXT;
            static constexpr auto VK_Dynamic_PatchControlPoints = VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT;
            static constexpr auto VK_Dynamic_LogicOp = VK_DYNAMIC_STATE_LOGIC_OP_EXT;
            static constexpr auto VK_Dynamic_ColorWriteEnable = VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT;
            static constexpr auto VK_Dynamic_DepthClampEnable = VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT;
            static constexpr auto VK_Dynamic_PolygonMode = VK_DYNAMIC_STATE_POLYGON_MODE_EXT;
            static constexpr auto VK_Dynamic_RasterizationSamples = VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT;
            static constexpr auto VK_Dynamic_SampleMask = VK_DYNAMIC_STATE_SAMPLE_MASK_EXT;
            static constexpr auto VK_Dynamic_AlphaToCoverageEnable = VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT;
            static constexpr auto VK_Dynamic_AlphaToOneEnable = VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT;
            static constexpr auto VK_Dynamic_LogicOpEnable = VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT;
            static constexpr auto VK_Dynamic_ColorBlendEnable = VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT;
            static constexpr auto VK_Dynamic_ColorBlendEquation = VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT;
            static constexpr auto VK_Dynamic_ColorWriteMask = VK_DYNAMIC_STATE_COLOR_WRITE_MASK_EXT;
            static constexpr auto VK_Dynamic_DepthClipEnable = VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT;
            static constexpr auto VK_Dynamic_SampleLocationsEnable = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT;
            static constexpr auto VK_Dynamic_ColorBlendAdvanced = VK_DYNAMIC_STATE_COLOR_BLEND_ADVANCED_EXT;
            static constexpr auto VK_Dynamic_ProvokingVertexMode = VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT;
            static constexpr auto VK_Dynamic_LineRasterizationMode = VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT;
            static constexpr auto VK_Dynamic_LineStippleEnable = VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT;
            static constexpr auto VK_Dynamic_DepthClipNegativeOneToOne = VK_DYNAMIC_STATE_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT;
            static constexpr auto VK_Dynamic_DepthClampRange = VK_DYNAMIC_STATE_DEPTH_CLAMP_RANGE_EXT;

            static constexpr auto VK_Primitive_PointList = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            static constexpr auto VK_Primitive_LineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            static constexpr auto VK_Primitive_LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            static constexpr auto VK_Primitive_TriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            static constexpr auto VK_Primitive_TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            static constexpr auto VK_Primitive_TriangleFan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
            static constexpr auto VK_Primitive_LineListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
            static constexpr auto VK_Primitive_LineStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
            static constexpr auto VK_Primitive_TriangleListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
            static constexpr auto VK_Primitive_TriangleStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
            static constexpr auto VK_Primitive_PatchList = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

            static constexpr auto VK_Polygon_Fill = VK_POLYGON_MODE_FILL;
            static constexpr auto VK_Polygon_Line = VK_POLYGON_MODE_LINE;
            static constexpr auto VK_Polygon_Point = VK_POLYGON_MODE_POINT;

            static constexpr auto VK_Cull_None = VK_CULL_MODE_NONE;
            static constexpr auto VK_Cull_Front = VK_CULL_MODE_FRONT_BIT;
            static constexpr auto VK_Cull_Back = VK_CULL_MODE_BACK_BIT;
            static constexpr auto VK_Cull_FrontAndBack = VK_CULL_MODE_FRONT_AND_BACK;

            static constexpr auto VK_FrontFace_CounterClockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            static constexpr auto VK_FrontFace_Clockwise = VK_FRONT_FACE_CLOCKWISE;

            static constexpr auto VK_LogicOp_Clear = VK_LOGIC_OP_CLEAR;
            static constexpr auto VK_LogicOp_And = VK_LOGIC_OP_AND;
            static constexpr auto VK_LogicOp_AndReverse = VK_LOGIC_OP_AND_REVERSE;
            static constexpr auto VK_LogicOp_Copy = VK_LOGIC_OP_COPY;
            static constexpr auto VK_LogicOp_AndInverted = VK_LOGIC_OP_AND_INVERTED;
            static constexpr auto VK_LogicOp_Noop = VK_LOGIC_OP_NO_OP;
            static constexpr auto VK_LogicOp_Xor = VK_LOGIC_OP_XOR;
            static constexpr auto VK_LogicOp_Or = VK_LOGIC_OP_OR;
            static constexpr auto VK_LogicOp_Nor = VK_LOGIC_OP_NOR;
            static constexpr auto VK_LogicOp_Equivalent = VK_LOGIC_OP_EQUIVALENT;
            static constexpr auto VK_LogicOp_Invert = VK_LOGIC_OP_INVERT;
            static constexpr auto VK_LogicOp_OrReverse = VK_LOGIC_OP_OR_REVERSE;
            static constexpr auto VK_LogicOp_CopyInverted = VK_LOGIC_OP_COPY_INVERTED;
            static constexpr auto VK_LogicOp_OrInverted = VK_LOGIC_OP_OR_INVERTED;
            static constexpr auto VK_LogicOp_Nand = VK_LOGIC_OP_NAND;
            static constexpr auto VK_LogicOp_Set = VK_LOGIC_OP_SET;

            static constexpr auto VK_Index_UInt16 = VK_INDEX_TYPE_UINT16;
            static constexpr auto VK_Index_UInt32 = VK_INDEX_TYPE_UINT32;
            static constexpr auto VK_Index_UInt8 = VK_INDEX_TYPE_UINT8;
            static constexpr auto VK_Index_None = VK_INDEX_TYPE_NONE_KHR;

            static constexpr auto VK_Access_IndirectCommandRead = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
            static constexpr auto VK_Access_IndexRead = VK_ACCESS_INDEX_READ_BIT;
            static constexpr auto VK_Access_VertexAttributeRead = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            static constexpr auto VK_Access_UniformRead = VK_ACCESS_UNIFORM_READ_BIT;
            static constexpr auto VK_Access_InputAttachmentRead = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
            static constexpr auto VK_Access_ShaderRead = VK_ACCESS_SHADER_READ_BIT;
            static constexpr auto VK_Access_ShaderWrite = VK_ACCESS_SHADER_WRITE_BIT;
            static constexpr auto VK_Access_ColorAttachmentRead = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            static constexpr auto VK_Access_ColorAttachmentWrite = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            static constexpr auto VK_Access_DepthStencilAttachmentRead = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            static constexpr auto VK_Access_DepthStencilAttachmentWrite = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            static constexpr auto VK_Access_TransferRead = VK_ACCESS_TRANSFER_READ_BIT;
            static constexpr auto VK_Access_TransferWrite = VK_ACCESS_TRANSFER_WRITE_BIT;
            static constexpr auto VK_Access_HostRead = VK_ACCESS_HOST_READ_BIT;
            static constexpr auto VK_Access_HostWrite = VK_ACCESS_HOST_WRITE_BIT;
            static constexpr auto VK_Access_MemoryRead = VK_ACCESS_MEMORY_READ_BIT;
            static constexpr auto VK_Access_MemoryWrite = VK_ACCESS_MEMORY_WRITE_BIT;
            static constexpr auto VK_Access_None = VK_ACCESS_NONE;
            static constexpr auto VK_Access_TransformFeedbackWrite = VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
            static constexpr auto VK_Access_TransformFeedbackCounterRead = VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;
            static constexpr auto VK_Access_TransformFeedbackCounterWrite = VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;
            static constexpr auto VK_Access_ConditionalRenderingRead = VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT;
            static constexpr auto VK_Access_ColorAttachmentReadNonCoherent = VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;
            static constexpr auto VK_Access_AccelerationStructureRead = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
            static constexpr auto VK_Access_AccelerationStructureWrite = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
            static constexpr auto VK_Access_FragmentDensityMapRead = VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
            static constexpr auto VK_Access_FragmentShadingRateAttachmentRead = VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
            static constexpr auto VK_Access_CommandPreprocessRead = VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_EXT;
            static constexpr auto VK_Access_CommandPreprocessWrite = VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_EXT;

            static constexpr auto VK_PipelineStage_TopOfPipe = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            static constexpr auto VK_PipelineStage_DrawIndirect = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
            static constexpr auto VK_PipelineStage_VertexInput = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
            static constexpr auto VK_PipelineStage_VertexShader = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            static constexpr auto VK_PipelineStage_TesselationControlShader = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
            static constexpr auto VK_PipelineStage_TesselationEvaluationShader = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
            static constexpr auto VK_PipelineStage_GeometryShader = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
            static constexpr auto VK_PipelineStage_FragmentShader = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            static constexpr auto VK_PipelineStage_EarlyFragmentTests = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            static constexpr auto VK_PipelineStage_LateFragmentTests = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            static constexpr auto VK_PipelineStage_ColorAttachmentOutput = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            static constexpr auto VK_PipelineStage_ComputeShader = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            static constexpr auto VK_PipelineStage_Transfer = VK_PIPELINE_STAGE_TRANSFER_BIT;
            static constexpr auto VK_PipelineStage_BottomOfPipe = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            static constexpr auto VK_PipelineStage_Host = VK_PIPELINE_STAGE_HOST_BIT;
            static constexpr auto VK_PipelineStage_AllGraphics = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            static constexpr auto VK_PipelineStage_AllCommands = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
            static constexpr auto VK_PipelineStage_None = VK_PIPELINE_STAGE_NONE;
            static constexpr auto VK_PipelineStage_TransformFeedback = VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;
            static constexpr auto VK_PipelineStage_ConditionalRendering = VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT;
            static constexpr auto VK_PipelineStage_AccelerationStructureBuild = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
            static constexpr auto VK_PipelineStage_RayTracingShader = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;
            static constexpr auto VK_PipelineStage_FragmentDensityProcess = VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;
            static constexpr auto VK_PipelineStage_FragmentShadingRateAttachment = VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
            static constexpr auto VK_PipelineStage_TaskShader = VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT;
            static constexpr auto VK_PipelineStage_MeshShader = VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT;
            static constexpr auto VK_PipelineStage_CommandPreprocess = VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_EXT;
            static constexpr auto VK_PipelineStage_EarlyAndLateFragmentTests = VK_PipelineStage_EarlyFragmentTests | VK_PipelineStage_LateFragmentTests;

            static constexpr auto VK_PipelineBindPoint_Graphics = VK_PIPELINE_BIND_POINT_GRAPHICS;
            static constexpr auto VK_PipelineBindPoint_Compute = VK_PIPELINE_BIND_POINT_COMPUTE;
            static constexpr auto VK_PipelineBindPoint_RayTracing = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;

            static constexpr auto VK_ShaderCreate_LinkStage = VK_SHADER_CREATE_LINK_STAGE_BIT_EXT;
            static constexpr auto VK_ShaderCreate_AllowVaryingSubgroupSize = VK_SHADER_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT_EXT;
            static constexpr auto VK_ShaderCreate_RequireFullSubgroups = VK_SHADER_CREATE_REQUIRE_FULL_SUBGROUPS_BIT_EXT;
            static constexpr auto VK_ShaderCreate_NoTaskShader = VK_SHADER_CREATE_NO_TASK_SHADER_BIT_EXT;
            static constexpr auto VK_ShaderCreate_DispatchBase = VK_SHADER_CREATE_DISPATCH_BASE_BIT_EXT;
            static constexpr auto VK_ShaderCreate_FragmentShadingRateAttachment = VK_SHADER_CREATE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_EXT;
            static constexpr auto VK_ShaderCreate_FragmentDensityMapAttachment = VK_SHADER_CREATE_FRAGMENT_DENSITY_MAP_ATTACHMENT_BIT_EXT;
            static constexpr auto VK_ShaderCreate_IndirectBindable = VK_SHADER_CREATE_INDIRECT_BINDABLE_BIT_EXT;

            static constexpr auto VK_ShaderCode_Binary = VK_SHADER_CODE_TYPE_BINARY_EXT;
            static constexpr auto VK_ShaderCode_SPIRV = VK_SHADER_CODE_TYPE_SPIRV_EXT;

            static constexpr auto VK_Resolve_None = VK_RESOLVE_MODE_NONE;
            static constexpr auto VK_Resolve_SampleZero = VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
            static constexpr auto VK_Resolve_Average = VK_RESOLVE_MODE_AVERAGE_BIT;
            static constexpr auto VK_Resolve_Min = VK_RESOLVE_MODE_MIN_BIT;
            static constexpr auto VK_Resolve_Max = VK_RESOLVE_MODE_MAX_BIT;

            static constexpr auto VK_PresentMode_Immediate = VK_PRESENT_MODE_IMMEDIATE_KHR;
            static constexpr auto VK_PresentMode_Mailbox = VK_PRESENT_MODE_MAILBOX_KHR;
            static constexpr auto VK_PresentMode_FIFO = VK_PRESENT_MODE_FIFO_KHR;
            static constexpr auto VK_PresentMode_FIFORelaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
            static constexpr auto VK_PresentMode_SharedDemandRefresh = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
            static constexpr auto VK_PresentMode_SharedContinuousRefresh = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
            static constexpr auto VK_PresentMode_FIFOLatestReady = VK_PRESENT_MODE_FIFO_LATEST_READY_EXT;

            static constexpr auto VK_CompositeAlpha_Opaque = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            static constexpr auto VK_CompositeAlpha_PreMultiplied = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
            static constexpr auto VK_CompositeAlpha_PostMultiplied = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
            static constexpr auto VK_CompositeAlpha_Inherit = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

            static constexpr auto VK_VertexInputRate_Vertex = VK_VERTEX_INPUT_RATE_VERTEX;
            static constexpr auto VK_VertexInputRate_Instance = VK_VERTEX_INPUT_RATE_INSTANCE;
        }
    }
}