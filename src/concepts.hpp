#pragma once

#include <range/v3/view/subrange.hpp>

#include <functional>

namespace mfl
{
    template<typename B>
    concept boolean =
    ranges::concepts::movable<std::remove_cvref_t<B>> &&
    requires(const std::remove_reference_t<B>& b1,
             const std::remove_reference_t<B>& b2, const bool a) {
        { b1 } -> ranges::concepts::convertible_to<bool>;
        { !b1 } -> ranges::concepts::convertible_to<bool>;
        { b1 && b2 } -> ranges::concepts::same_as<bool>;
        { b1 &&  a } -> ranges::concepts::same_as<bool>;
        {  a && b2 } -> ranges::concepts::same_as<bool>;
        { b1 || b2 } -> ranges::concepts::same_as<bool>;
        { b1 ||  a } -> ranges::concepts::same_as<bool>;
        {  a || b2 } -> ranges::concepts::same_as<bool>;
        { b1 == b2 } -> ranges::concepts::convertible_to<bool>;
        { b1 ==  a } -> ranges::concepts::convertible_to<bool>;
        {  a == b2 } -> ranges::concepts::convertible_to<bool>;
        { b1 != b2 } -> ranges::concepts::convertible_to<bool>;
        { b1 !=  a } -> ranges::concepts::convertible_to<bool>;
        {  a != b2 } -> ranges::concepts::convertible_to<bool>;
    };

    template<typename F, typename... Args>
    concept invocable = requires(F&& f, Args&&... args) {
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    };

    template <typename F, typename... Args>
    concept predicate = invocable<F, Args...> && boolean<std::invoke_result_t<F, Args...>>;

    template <typename Rng, typename T>
    concept range_of = ranges::range<Rng> && ranges::concepts::same_as<ranges::range_value_type_t<Rng>, T>;

    template <typename Rng, typename T>
    concept range_of_convertible_to =
        ranges::range<Rng> && ranges::concepts::convertible_to<ranges::range_value_type_t<Rng>, T>;
}
