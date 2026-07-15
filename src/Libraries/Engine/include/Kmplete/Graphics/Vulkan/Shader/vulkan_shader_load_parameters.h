#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/ShaderCompiler/compile.h"

#include <variant>


namespace Kmplete
{
    namespace Graphics
    {
        //! Enumeration of possible sources of shaders objects/modules
        enum class ShaderSourceType
        {
            SourceFile,
            SourceCode,
            BinaryFile,
            BinaryCode
        };
        //--------------------------------------------------------------------------


        using ShaderSource = std::variant<Filepath, String, BinaryBuffer32>;

        //! Helper struct of shader object/module loading parameters
        //! @see VulkanShaderManager
        struct ShaderLoadParameters
        {
            StringID sid;
            ShaderSource source;
            ShaderSourceType sourceType;
            ShaderCompiler::ShaderType shaderType;
        };
        //--------------------------------------------------------------------------
    }
}