#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        String GraphicsBackendTypeToString(GraphicsBackendType type) noexcept
        {
            switch (type)
            {
            case GraphicsBackendType::Vulkan:
                return VulkanStr;
            default:
                return UnknownStr;
            }
        }
        //--------------------------------------------------------------------------

        GraphicsBackendType StringToGraphicsBackendType(const String& string) noexcept
        {
            if (string == VulkanStr)
            {
                return GraphicsBackendType::Vulkan;
            }

            return GraphicsBackendType::Unknown;
        }
        //--------------------------------------------------------------------------


        UInt32 ShaderDataTypeSize(ShaderDataType type) noexcept
        {
            switch (type)
            {
                case ShaderDataType::Bool:     return 1;
                case ShaderDataType::Int1:     return 4;
                case ShaderDataType::Int2:     return 4 * 2;
                case ShaderDataType::Int3:     return 4 * 3;
                case ShaderDataType::Int4:     return 4 * 4;
                case ShaderDataType::Float1:   return 4;
                case ShaderDataType::Float2:   return 4 * 2;
                case ShaderDataType::Float3:   return 4 * 3;
                case ShaderDataType::Float4:   return 4 * 4;
                case ShaderDataType::Mat3:     return 4 * 3 * 3;
                case ShaderDataType::Mat4:     return 4 * 4 * 4;
            }

            KMP_LOG_ERROR_FN("GraphicsBase: unknown ShaderDataType");
            return 0;
        }
        //--------------------------------------------------------------------------

        UInt32 GetComponentCount(ShaderDataType type) noexcept
        {
            switch (type)
            {
                case ShaderDataType::Bool:     return 1;
                case ShaderDataType::Int1:     return 1;
                case ShaderDataType::Int2:     return 2;
                case ShaderDataType::Int3:     return 3;
                case ShaderDataType::Int4:     return 4;
                case ShaderDataType::Float1:   return 1;
                case ShaderDataType::Float2:   return 2;
                case ShaderDataType::Float3:   return 3;
                case ShaderDataType::Float4:   return 4;
                case ShaderDataType::Mat3:     return 3 * 3;
                case ShaderDataType::Mat4:     return 4 * 4;
            }

            KMP_LOG_ERROR_FN("GraphicsBase: unknown ShaderDataType");
            return 0;
        }
        //--------------------------------------------------------------------------

        BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
            : _elements(elements)
            , _stride(0)
        {
            _CalculateLayout();
        }
        //--------------------------------------------------------------------------

        UInt32 BufferLayout::GetStride() const noexcept
        {
            return _stride;
        }
        //--------------------------------------------------------------------------

        const Vector<BufferElement>& BufferLayout::GetElements() const noexcept
        {
            return _elements;
        }
        //--------------------------------------------------------------------------

        void BufferLayout::_CalculateLayout()
        {
            size_t offset = 0;
            _stride = 0;

            for (auto& element : _elements)
            {
                const auto elementSize = ShaderDataTypeSize(element.type);
                element.offset = offset;
                offset += elementSize;
                _stride += elementSize;
            }
        }
        //--------------------------------------------------------------------------
    }
}