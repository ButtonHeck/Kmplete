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


            //! The processor class for compiling shaders from source files to SPIRV binary. Supports two work modes:
            //! 1) Direct mode - pass shaders source file paths, SPIRV destination file paths and shader types
            //! directly as arguments lists. In this mode all three lists' lengths should match each other in order
            //! to work correctly. e.g.:
            //! --mode Direct --source_files "file1.vert" "file2.frag" --destination-files "outFile1.vert.spv" "outFile2.frag.spv" --shader_types "Vertex" "Fragment"
            //!                               1            2                                1                   2                                  1        2
            //! 2) Json mode - pass all the parameters for shaders compilation in json file(s) each of which is in the following format:
            //! {
            //!     "Shaders": [
            //!         {
            //!             "Source": "path_to_shader1.vert",
            //!             "Destination": "path_to_shader1_output.vert.spv",
            //!             "ShaderType": "Vertex"
            //!         },
            //!         {
            //!             "Source": "path_to_shader2.frag",
            //!             "Destination": "path_to_shader2_output.frag.spv",
            //!             "ShaderType": "Fragment"
            //!         }
            //!     ]
            //! }
            //! 
            //! e.g:
            //! --mode Json --source_files "path_to_shaders1.json" "path_to_shaders2.json"
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