#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Enumeration of image color channels
        enum ImageChannels
        {
            Unknown = 0,
            Grey,
            GreyAlpha,
            RGB,
            RGBAlpha
        };
        //--------------------------------------------------------------------------


        //! Enumeration of the graphics backend APIs supported be the engine
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


        //! Global constant value for number of buffers (or concurrent frames) used
        //! during rendering (1 buffer/frame is used for presentation and other(s) are used
        //! for rendering)
        static constexpr auto NumConcurrentFrames = 2;


        //! Enumeration of primitive types used in shaders, e.g. in a shader string:
        //! layout (location = 0) in vec3 inPos;
        //! vec3 maps to Float3
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

        //! A shader's attribute description representation stored in a vertex buffer, e.g. in a shader string:
        //! layout (location = 0) in vec3 inPos;
        //! 1) "type" == Float3
        //! 2) "location" == 0
        //! 3) "offset" is calculated manually in vertex buffer
        //! 4) "normalized" depends on the value provided
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

        //! A collection of BufferElements stored in a vertex buffer.
        //! These elements may not be ordered by their location values, but supposed to be
        //! contiguously stored in a vertex buffer (uses tightly packed approach)
        class KMP_API BufferLayout
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE_COPYABLE()

        public:
            BufferLayout() = default;
            explicit BufferLayout(InitializerList<BufferElement> elements, bool isInstanced = false);
            ~BufferLayout() = default;

            KMP_NODISCARD UInt32 GetStride() const noexcept;
            KMP_NODISCARD bool IsInstanced() const noexcept;
            KMP_NODISCARD const Vector<BufferElement>& GetElements() const noexcept;

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