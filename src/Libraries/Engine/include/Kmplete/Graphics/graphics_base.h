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
            UInt32 location;
            bool normalized;

            BufferElement() = default;
            BufferElement(ShaderDataType dataType, UInt32 loc, bool isNormalized = false)
                : type(dataType)
                , offset(0)
                , location(loc)
                , normalized(isNormalized)
            {}
        };
        //--------------------------------------------------------------------------

        //TODO: comments
        class BufferLayout
        {
        public:
            BufferLayout() = default;
            KMP_API explicit BufferLayout(InitializerList<BufferElement> elements, bool isInstanced = false);

            KMP_NODISCARD KMP_API UInt32 GetStride() const noexcept;
            KMP_NODISCARD KMP_API bool IsInstanced() const noexcept;
            KMP_NODISCARD KMP_API const Vector<BufferElement>& GetElements() const noexcept;

        private:
            void _CalculateLayout();

        private:
            Vector<BufferElement> _elements;
            UInt32 _stride = 0;
            bool _instanced = false;
        };
        //--------------------------------------------------------------------------
    }
}