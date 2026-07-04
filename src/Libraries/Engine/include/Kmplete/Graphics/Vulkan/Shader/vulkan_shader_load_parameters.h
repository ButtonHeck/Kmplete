#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/ShaderCompiler/compile.h"

#include <variant>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        enum class ShaderSourceType
        {
            SourceFile,
            SourceCode,
            BinaryFile,
            BinaryCode
        };
        //--------------------------------------------------------------------------


        using ShaderSource = std::variant<Filepath, String, BinaryBuffer32>;

        //TODO: comments
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