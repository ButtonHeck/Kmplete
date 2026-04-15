#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Assets/assets_compiler_interface.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class JsonDocument;


    namespace Assets
    {
        namespace Compiler
        {
            //! Representation of the AssetsCompiler program arguments:
            //! --input_file_name
            //! --output_file_name
            //! --logging
            struct CompilerParameters
            {
                Filepath sourceJsonFile;
                Filepath outputDataFile;
                bool logging = false;
            };
            //--------------------------------------------------------------------------


            //! The processor class for gathering assets from files and compiling them.
            //! The compiler takes json file that contains information of assets to compile
            //! such as filepath, name (converted to StringID) and type of a single asset. Then it processses all the
            //! metadata and put both assets headers and its binaries to the output file.
            //! At the moment this class only capable of parsing single input file and writing
            //! single output file. Duplication of StringIDs leads to an error and stops further processing.
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
                mutable HashSet<StringID> _processedSids;
            };
            //--------------------------------------------------------------------------
        }
    }
}