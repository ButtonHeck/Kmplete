#include "Kmplete/ShaderCompiler/compile.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <shaderc/shaderc.hpp>

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
    #include <crtdbg.h>
#endif


namespace Kmplete
{
    namespace ShaderCompiler
    {
        namespace
        {
            String ShaderTypeToString(ShaderType shaderType)
            {
                switch (shaderType)
                {
                case ShaderType::Vertex:
                    return "Vertex";
                case ShaderType::Fragment:
                    return "Fragment";
                case ShaderType::Compute:
                    return "Compute";
                case ShaderType::RayTracing:
                    return "RayTracing";
                default:
                    return "Unknown";
                }
            }
            //--------------------------------------------------------------------------

            shaderc_shader_kind ShaderTypeToShadercNative(ShaderType shaderType)
            {
                switch (shaderType)
                {
                case ShaderType::Vertex:
                    return shaderc_glsl_vertex_shader;
                case ShaderType::Fragment:
                    return shaderc_glsl_fragment_shader;
                case ShaderType::Compute:
                    return shaderc_glsl_compute_shader;
                default:
                    return shaderc_glsl_raygen_shader;
                }
            }
            //--------------------------------------------------------------------------
        }


        BinaryBuffer32 CompileGLSLToSpirvFromSource(const String& sourceName, ShaderType shaderType, const String& shaderCode, bool optimize /*= true*/) KMP_PROFILING(ProfileLevelImportant)
        {
            if (shaderCode.empty())
            {
                KMP_LOG_ERROR_FN("CompileGLSLToSpirvFromSource: cannot compile shader - code is empty (compiling '{}' shader named '{}')", ShaderTypeToString(shaderType), sourceName);
                return BinaryBuffer32();
            }

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC) && defined (KMP_CONFIG_TYPE_DEBUG)
            // bypass intentional memory leak due to unreleased GlslangInitializer
            // https://github.com/google/shaderc/issues/1052
            int oldFlag = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            int newFlag = oldFlag & ~_CRTDBG_ALLOC_MEM_DF;
            _CrtSetDbgFlag(newFlag);
#endif
            shaderc::Compiler compiler;
#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC) && defined (KMP_CONFIG_TYPE_DEBUG)
            _CrtSetDbgFlag(oldFlag);
#endif

            if (!compiler.IsValid())
            {
                KMP_LOG_ERROR_FN("CompileGLSLToSpirvFromSource: failed to create shaderc compiler instance (compiling '{}' shader named '{}')", ShaderTypeToString(shaderType), sourceName);
                return BinaryBuffer32();
            }

            shaderc::CompileOptions options;
            if (optimize)
            {
                options.SetOptimizationLevel(shaderc_optimization_level_performance);
            }

            const auto result = compiler.CompileGlslToSpv(shaderCode, ShaderTypeToShadercNative(shaderType), sourceName.c_str(), options);
            if (result.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                KMP_LOG_ERROR_FN("CompileGLSLToSpirvFromSource: shader compilation failed (compiling '{}' shader named '{}')\n\t{}", ShaderTypeToString(shaderType), sourceName, result.GetErrorMessage());
                return BinaryBuffer32();
            }

            KMP_LOG_INFO_FN("CompileGLSLToSpirvFromSource: successfully compiled '{}' shader named '{}'", ShaderTypeToString(shaderType), sourceName);
            return BinaryBuffer32({ result.cbegin(), result.cend() });
        }}
        //--------------------------------------------------------------------------

        BinaryBuffer32 CompileGLSLToSpirvFromFile(const String& sourceName, ShaderType shaderType, const Filepath& shaderFile, bool optimize /*= true*/) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto shaderCode = Filesystem::ReadFileAsText(shaderFile);
            if (shaderCode.empty())
            {
                KMP_LOG_ERROR_FN("CompileGLSLToSpirvFromFile: failed to read shader code from '{}' (compiling '{}' shader named '{}')", shaderFile, ShaderTypeToString(shaderType), sourceName);
                return BinaryBuffer32();
            }

            return CompileGLSLToSpirvFromSource(sourceName, shaderType, shaderCode, optimize);
        }}
        //--------------------------------------------------------------------------
    }
}