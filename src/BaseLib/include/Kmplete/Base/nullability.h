#pragma once


namespace Kmplete
{
    //! Syntatic sugar for marking raw pointers that might become nullptr
    //! during their lifetime when used as members or part of other members
    //! or might be nullptr when used as a function return value or a function argument,
    //! e.g. Image might return nullptr pixel buffer if its loading failed or this image was moved.
    template <typename T>
    using Nullable = T;

    //! Syntatic sugar for marking raw pointers that somehow "guaranteed" not to be nullptr,
    //! it is a programmer's responsibility to keep such a contract valid.
    //! e.g. WindowGlfw is either "guarantees" to keep its UserData alive if the window itself was successfully created
    //! or the entire window creation failed and thus it becomes pointless to get UserData pointer
    template <typename T>
    using NonNull = T;
}
