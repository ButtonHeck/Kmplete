#pragma once

#include <functional>


namespace Kmplete
{
    //! Codestyle-friendly function alias
    template<typename FuncType>
    using Function = std::function<FuncType>;
}