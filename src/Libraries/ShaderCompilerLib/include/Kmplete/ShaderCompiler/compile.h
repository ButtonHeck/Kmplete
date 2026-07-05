#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"


namespace Kmplete
{
    namespace ShaderCompiler
    {
        enum class ShaderType
        {
            Vertex,
            Fragment,
            Geometry,
            Compute,
            RayTracing
        };
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API String ShaderTypeToString(ShaderType shaderType) noexcept;
        KMP_NODISCARD KMP_API Optional<ShaderType> StringToShaderType(const String& shaderTypeString) noexcept;

        KMP_NODISCARD KMP_API BinaryBuffer32 CompileGLSLToSpirvFromSource(const String& sourceName, ShaderType shaderType, const String& shaderCode, bool optimize = true);
        KMP_NODISCARD KMP_API BinaryBuffer32 CompileGLSLToSpirvFromFile(const String& sourceName, ShaderType shaderType, const Filepath& shaderFile, bool optimize = true);
    }
}