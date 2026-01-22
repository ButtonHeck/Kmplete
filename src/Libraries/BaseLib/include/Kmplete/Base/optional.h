#pragma once

#include <optional>
#include <functional>


namespace Kmplete
{
    //! Codestyle-friendly aliases

    template<typename T>
    using Optional = std::optional<T>;

    template<typename T>
    using Ref = std::reference_wrapper<T>;

    template<typename T>
    using OptionalRef = Optional<Ref<T>>;
}