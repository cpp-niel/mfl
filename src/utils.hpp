#pragma once

#include <memory>
#include <utility>
#include <variant>

namespace mfl
{
    template <class... Ts>
    struct overload : Ts...
    {
        using Ts::operator()...;
    };

    template <class... Ts>
    overload(Ts...) -> overload<Ts...>;

    template <typename T>
    class recursive_wrapper
    {
    public:
        recursive_wrapper() = default;

        // This is one of the rare cases where the implicit conversions are intentional. The recursive
        // wrapper allows us to declare a variant where one of the types in the variant has
        // a member of the variant type. For instance the node_variant type has a box member which holds
        // node_variants. To achieve this the variant actually has a recursive_wrapper<box> alternative
        // and box is forward declared. But we want the wrapper to behave like a box, so we allow
        // implicit conversions.
        recursive_wrapper(T&& x) : p_(std::make_shared<T>(std::forward<T>(x))) {}  // NOLINT(hicpp-explicit-conversions)
        operator const T&() const { return *p_; }                                  // NOLINT(hicpp-explicit-conversions)
        operator T&() { return *p_; }                                              // NOLINT(hicpp-explicit-conversions)

    private:
        std::shared_ptr<T> p_;
    };

    template <typename T, typename Variant>
    struct is_variant_alternative;

    template <typename T, typename... Args>
    struct is_variant_alternative<T, std::variant<Args...>>
        : std::bool_constant<(std::is_convertible_v<T, Args> || ...)>
    {
    };
}