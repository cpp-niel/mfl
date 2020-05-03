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
    overload(Ts...)->overload<Ts...>;

    template <typename T>
    class recursive_wrapper
    {
    public:
        recursive_wrapper() = default;
        recursive_wrapper(T&& x) : p_(std::make_shared<T>(std::forward<T>(x))) {}
        operator const T&() const { return *p_; }
        operator T&() { return *p_; }

    private:
        std::shared_ptr<T> p_;
    };

    template <typename T, typename Variant>
    struct is_variant_alternative;

    template <typename T, typename... Args>
    struct is_variant_alternative<T, std::variant<Args...>> : std::bool_constant<(std::is_convertible_v<T, Args> || ...)>
    {
    };
}