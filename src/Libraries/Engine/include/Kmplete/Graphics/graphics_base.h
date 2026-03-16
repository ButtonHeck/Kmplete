#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace Graphics
    {
        enum ImageChannels
        {
            Unknown = 0,
            Grey,
            GreyAlpha,
            RGB,
            RGBAlpha
        };
        //--------------------------------------------------------------------------


        enum class GraphicsBackendType
        {
            Vulkan,
            Unknown
        };
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API String GraphicsBackendTypeToString(GraphicsBackendType type) noexcept;
        KMP_NODISCARD KMP_API GraphicsBackendType StringToGraphicsBackendType(const String& string) noexcept;

        static constexpr auto VulkanStr = "Vulkan";
        static constexpr auto UnknownStr = "Unknown";
        static constexpr auto DefaultAPIStr = VulkanStr;
        //--------------------------------------------------------------------------


        static constexpr auto NumConcurrentFrames = 2;


        //TODO: comments
        enum class ShaderDataType
        {
            None = 0,
            Bool,
            Int1, Int2, Int3, Int4,
            Float1, Float2, Float3, Float4,
            Mat3, Mat4
        };
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API UInt32 ShaderDataTypeSize(ShaderDataType type) noexcept;
        KMP_NODISCARD KMP_API UInt32 GetComponentCount(ShaderDataType type) noexcept;

        //TODO: comments
        struct BufferElement
        {
            ShaderDataType type;
            size_t offset;
            bool normalized;

            BufferElement() = default;
            explicit BufferElement(ShaderDataType dataType, bool isNormalized = false)
                : type(dataType)
                , offset(0)
                , normalized(isNormalized)
            {}
        };
        //--------------------------------------------------------------------------

        //TODO: comments
        class BufferLayout
        {
        public:
            BufferLayout() = default;
            KMP_API explicit BufferLayout(std::initializer_list<BufferElement> elements);

            KMP_NODISCARD KMP_API UInt32 GetStride() const noexcept;
            KMP_NODISCARD KMP_API const Vector<BufferElement>& GetElements() const noexcept;

        private:
            void _CalculateLayout();

        private:
            Vector<BufferElement> _elements;
            UInt32 _stride = 0;
        };
        //--------------------------------------------------------------------------
    }
}