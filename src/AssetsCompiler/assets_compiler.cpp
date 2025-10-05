#include "assets_compiler.h"

#include "Kmplete/Json/json_document.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Assets
    {
        namespace Compiler
        {
            AssetsCompiler::AssetsCompiler(CompilerParameters&& parameters) noexcept
                : _parameters(parameters)
            {}
            //--------------------------------------------------------------------------

            int AssetsCompiler::Run() const
            {
                auto assetsDescriptionJson = JsonDocument(_parameters.sourceFile);
                if (assetsDescriptionJson.HasError())
                {
                    KMP_LOG_ERROR("assets description parsing failed");
                    return ReturnCode::InputFileParsingError;
                }

                const auto assetCount = assetsDescriptionJson.StartGetArray(JsonConfigurationTopLevelName);
                if (assetCount == 0)
                {
                    KMP_LOG_ERROR("required array '{}' not found or this array is empty", JsonConfigurationTopLevelName);
                    return ReturnCode::InputFileProcessingError;
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------
        }
    }
}