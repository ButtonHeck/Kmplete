#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Assets/assets_compiler_interface.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class JsonDocument;


    namespace Assets
    {
        namespace Compiler
        {
            struct CompilerParameters
            {
                Filepath sourceFile;
                Filepath outputFile;
                bool logging = false;
            };
            //--------------------------------------------------------------------------


            class AssetsCompiler
            {
                KMP_LOG_CLASSNAME(AssetsCompiler)

            public:
                explicit AssetsCompiler(CompilerParameters&& parameters) noexcept;

                KMP_NODISCARD ReturnCode Run() const;

            private:
                struct WriteBufferState
                {
                    UInt64 assetDataBufferOffset;
                    UInt64 assetBufferSizeCurrentOffset;
                    UInt64 assetBufferOffsetCurrentOffset;
                };

            private:
                KMP_NODISCARD ReturnCode _WriteHeaders(JsonDocument& sourceJson, AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, BinaryBuffer& assetsTypes) const;
                KMP_NODISCARD ReturnCode _WriteHeader(UInt32 assetIndex, JsonDocument& sourceJson, std::ofstream& outputFile, FilepathVector& assetsFilepaths, BinaryBuffer& assetsTypes) const;
                KMP_NODISCARD ReturnCode _WriteBinaries(AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, BinaryBuffer& assetsTypes) const;
                KMP_NODISCARD ReturnCode _WriteBinary(std::ofstream& outputFile, const Filepath& filepath, WriteBufferState& writeState, const String& assetTypeName) const;

            private:
                const CompilerParameters _parameters;
                mutable HashSet<Utils::StringID> _processedSids;
            };
            //--------------------------------------------------------------------------
        }
    }
}