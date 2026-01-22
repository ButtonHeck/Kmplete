#pragma once

#include <type_traits>
#include <concepts>


namespace Kmplete
{
    //! Codestyle-friendly aliases for commonly used type traits

    template<typename T1, typename T2>
    using Same = std::is_same<T1, T2>;

    template<typename T1, typename... RestTypes>
    using Both = std::conjunction<T1, RestTypes...>;

    template<typename T1, typename... RestTypes>
    using Either = std::disjunction<T1, RestTypes...>;

    template<typename Type, typename... CandidateTypes>
    concept IsAnyOfType = (Same<Type, CandidateTypes>::value || ...);


    template<typename T>
    using IsArithmetic = std::is_arithmetic<T>;

    template<typename T>
    using IsIntegral = std::is_integral<T>;

    template<typename T>
    using IsReal = std::is_floating_point<T>;


    template<typename T>
    using IsMoveConstructible = std::is_move_constructible<T>;

    template<typename T>
    using IsMoveAssignable = std::is_move_assignable<T>;

    template<typename T>
    using IsTriviallyCopyable = std::is_trivially_copyable<T>;

    template<typename T>
    using IsTriviallyMoveConstructible = std::is_trivially_move_constructible<T>;

    template<typename T>
    using IsTriviallyMoveAssignable = std::is_trivially_move_assignable<T>;


    template<typename Fn, typename... Params>
    using IsInvocable = std::is_invocable<Fn, Params...>;

    template<typename Fn, typename... Params>
    using InvokeResult = std::invoke_result_t<Fn, Params...>;


    template<typename BaseClass, typename DerivedClass>
    using IsBaseClass = std::is_base_of<BaseClass, DerivedClass>;
}