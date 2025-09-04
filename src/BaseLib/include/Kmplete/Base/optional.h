#pragma once

#include <optional>
#include <functional>

namespace Kmplete
{
    template<typename T>
    using Optional = std::optional<T>;

    template<typename T>
    using Ref = std::reference_wrapper<T>;

    template<typename T>
    using OptionalRef = Optional<Ref<T>>;
}