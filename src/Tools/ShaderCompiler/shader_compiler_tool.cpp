#include "shader_compiler_tool.h"

#include "Kmplete/Json/json_document.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace ShaderCompiler
    {
        namespace ShaderCompilerTool
        {
            ShaderCompilerProcessor::ShaderCompilerProcessor(CompilerParameters&& parameters) noexcept
                : _parameters(parameters)
            {}
            //--------------------------------------------------------------------------

            ReturnCode ShaderCompilerProcessor::Run() const
            {
                ReturnCode returnCode = ReturnCode::Ok;
                if (_parameters.workMode == CompilerWorkModeDirect)
                {
                    returnCode = _CompileFromSourceFiles();
                }
                else if (_parameters.workMode == CompilerWorkModeJson)
                {
                    returnCode = _CompileFromJson();
                }

                return returnCode;
            }
            //--------------------------------------------------------------------------

            ReturnCode ShaderCompilerProcessor::_CompileFromSourceFiles() const
            {
                KMP_LOG_INFO("start compiling from source files...");

                for (size_t i = 0; i < _parameters.shaderSourceFiles.size(); i++)
                {
                    const auto& shaderSourceFile = _parameters.shaderSourceFiles[i];
                    const auto& shaderBinaryDestinationFile = _parameters.shaderBinaryDestinationFiles[i];
                    const auto shaderType = _parameters.shaderTypes[i];

                    const auto compilationReturnCode = _CompileSingleShader(shaderSourceFile, shaderBinaryDestinationFile, shaderType);
                    if (compilationReturnCode != ReturnCode::Ok)
                    {
                        return compilationReturnCode;
                    }
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode ShaderCompilerProcessor::_CompileFromJson() const
            {
                KMP_LOG_INFO("start compiling from json files...");

                for (size_t i = 0; i < _parameters.sourceJsonFiles.size(); i++)
                {
                    const auto processJsonReturnCode = _ProcessSingleJson(_parameters.sourceJsonFiles[i]);
                    if (processJsonReturnCode != ReturnCode::Ok)
                    {
                        return processJsonReturnCode;
                    }
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode ShaderCompilerProcessor::_ProcessSingleJson(const Filepath& sourceJsonFile) const
            {
                auto sourceJson = JsonDocument(sourceJsonFile);
                if (sourceJson.HasError())
                {
                    KMP_LOG_ERROR("shader description parsing failed - {}", sourceJson.ErrorDescription());
                    return ReturnCode::ShaderJsonFileParsingError;
                }

                const auto shaderSourcesCount = sourceJson.StartGetArray(JsonConfigurationTopLevelName);
                if (shaderSourcesCount == 0)
                {
                    KMP_LOG_ERROR("required array '{}' not found or this array is empty", JsonConfigurationTopLevelName);
                    return ReturnCode::ShaderJsonFileFormatError;
                }

                for (auto shaderIndex = 0; shaderIndex < shaderSourcesCount; shaderIndex++)
                {
                    const auto processShaderReturnCode = _ProcessSingleShader(sourceJson, shaderIndex);
                    if (processShaderReturnCode != ReturnCode::Ok)
                    {
                        return processShaderReturnCode;
                    }
                }

                if (!sourceJson.EndGetArray())
                {
                    KMP_LOG_ERROR("failed to end shader compiling descriptions array");
                    return ReturnCode::ShaderJsonFileFormatError;
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode ShaderCompilerProcessor::_ProcessSingleShader(JsonDocument& sourceJson, KMP_MB_UNUSED int shaderIndex) const
            {
                if (!sourceJson.StartGetObject(shaderIndex))
                {
                    KMP_LOG_ERROR("failed to get shader description json object at index {}", shaderIndex);
                    return ReturnCode::ShaderJsonFileFormatError;
                }

                const auto shaderSourceFilename = sourceJson.GetString(JsonConfigurationSourceFileStr);
                if (shaderSourceFilename.empty())
                {
                    KMP_LOG_ERROR("failed to get shader description source filename at index {}", shaderIndex);
                    return ReturnCode::ShaderJsonFileDataError;
                }
                if (!Filesystem::FilepathExists(shaderSourceFilename) || !Filesystem::IsFile(shaderSourceFilename))
                {
                    KMP_LOG_ERROR("shader source file '{}', does not exist of is not of a file type", shaderSourceFilename);
                    return ReturnCode::ShaderJsonFileDataError;
                }

                const auto shaderBinaryDestinationFilename = sourceJson.GetString(JsonConfigurationDestinationFileStr);
                if (shaderBinaryDestinationFilename.empty())
                {
                    KMP_LOG_ERROR("failed to get shader description binary destination filename at index {}", shaderIndex);
                    return ReturnCode::ShaderJsonFileDataError;
                }

                const auto shaderTypeStr = sourceJson.GetString(JsonConfigurationShaderTypeStr);
                const auto shaderType = StringToShaderType(shaderTypeStr);
                if (!shaderType.has_value())
                {
                    KMP_LOG_ERROR("failed to get shader description shader type at index {}", shaderIndex);
                    return ReturnCode::ShaderJsonFileDataError;
                }

                if (!sourceJson.EndGetObject())
                {
                    KMP_LOG_ERROR("failed to end shader description json object at index {}", shaderIndex);
                    return ReturnCode::ShaderJsonFileFormatError;
                }

                const auto compilationReturnCode = _CompileSingleShader(shaderSourceFilename, shaderBinaryDestinationFilename, shaderType.value());
                if (compilationReturnCode != ReturnCode::Ok)
                {
                    return compilationReturnCode;
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode ShaderCompilerProcessor::_CompileSingleShader(const Filepath& source, const Filepath& destination, ShaderType shaderType) const
            {
                const auto shaderSource = Filesystem::ReadFileAsText(source);
                if (shaderSource.empty())
                {
                    KMP_LOG_ERROR("shader source is empty from '{}'", source);
                    return ReturnCode::ShaderSourceIsEmpty;
                }

                const auto shaderBinary = CompileGLSLToSpirvFromFile(Filesystem::ToNativeString(source.filename()), shaderType, source, "optimize"_true);
                if (shaderBinary.empty())
                {
                    KMP_LOG_ERROR("shader binary buffer is empty from '{}'", source);
                    return ReturnCode::ShaderBinaryBufferIsEmpty;
                }

                const auto writeSuccessful = Filesystem::WriteFile(destination, shaderBinary, "append"_false);
                if (!writeSuccessful)
                {
                    KMP_LOG_ERROR("shader binary failed to save to a file '{}'", destination);
                    return ReturnCode::ShaderBinaryWriteFileFailed;
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------
        }
    }
}