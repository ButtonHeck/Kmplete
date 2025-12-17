#pragma once

#include <functional>


namespace Kmplete
{
    template<typename FuncType>
    using Function = std::function<FuncType>;
}