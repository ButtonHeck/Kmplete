#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/ShaderCompiler/shader_compiler_tool_interface.h"
#include "Kmplete/ShaderCompiler/compile.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class JsonDocument;


    namespace ShaderCompiler
    {
        namespace ShaderCompilerTool
        {
            //! Representation of the ShaderCompilerTool program arguments:
            //! --mode [Direct/Json]
            //! --source_files
            //! --destination_files
            //! --shader_types
            //! --logging
            struct CompilerParameters
            {
                String workMode;
                FilepathVector sourceJsonFiles;
                FilepathVector shaderSourceFiles;
                FilepathVector shaderBinaryDestinationFiles;
                Vector<ShaderType> shaderTypes;
                bool logging = false;
            };
            //--------------------------------------------------------------------------


            //TODO: comments
            class ShaderCompilerProcessor
            {
                KMP_LOG_CLASSNAME(ShaderCompilerProcessor)

            public:
                explicit ShaderCompilerProcessor(CompilerParameters&& parameters) noexcept;
                ~ShaderCompilerProcessor() = default;

                KMP_NODISCARD ReturnCode Run() const;

            private:
                KMP_NODISCARD ReturnCode _CompileFromSourceFiles() const;
                KMP_NODISCARD ReturnCode _CompileFromJson() const;

                KMP_NODISCARD ReturnCode _ProcessSingleJson(const Filepath& sourceJsonFile) const;
                KMP_NODISCARD ReturnCode _ProcessSingleShader(JsonDocument& sourceJson, int shaderIndex) const;
                KMP_NODISCARD ReturnCode _CompileSingleShader(const Filepath& source, const Filepath& destination, ShaderType shaderType) const;

            private:
                const CompilerParameters _parameters;
            };
            //--------------------------------------------------------------------------
        }
    }
}