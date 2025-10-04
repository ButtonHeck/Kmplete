#include "assets_compiler.h"


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
                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------
        }
    }
}