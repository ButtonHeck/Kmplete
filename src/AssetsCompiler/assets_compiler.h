#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
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
                KMP_NODISCARD ReturnCode WriteHeaderData(JsonDocument& sourceJson, AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, Vector<UByte>& assetsTypes) const;
                KMP_NODISCARD ReturnCode WriteAssetsData(AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, Vector<UByte>& assetsTypes) const;

            private:
                const CompilerParameters _parameters;
            };
            //--------------------------------------------------------------------------
        }
    }
}