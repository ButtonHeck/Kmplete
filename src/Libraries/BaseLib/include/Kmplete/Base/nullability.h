#pragma once

#include <gsl/pointers>


namespace Kmplete
{
    //! Syntatic sugar for marking raw pointers that might become nullptr
    //! during their lifetime when used as members or part of other members
    //! or might be nullptr when used as a function return value or a function argument,
    //! e.g. Image might return nullptr pixel buffer if its loading failed or this image was moved.
    template <typename T>
    using Nullable = T;


    template <typename T>
    using NonNull = gsl::not_null<T>;
}
