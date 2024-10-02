#pragma once
#include <array>
#include <concepts>
#include <cstddef>
#include <string_view>
#include <type_traits>
#include <vector>

#ifndef CONCEPT_CHECK
namespace My {
template <typename T>
struct CheckConstexprHelper {
    static constexpr T v{};
    using Type = T;
};

template <typename T>
concept IsCompileTimeType = requires { typename CheckConstexprHelper<T>::Type; };

template <typename T, T t = {}>
struct IsLiteralTypeHelper {
    using Type = T;
};

template <typename T, typename U = std::void_t<>>
struct IsLiteralType {
    static constexpr bool value = false;
};

template <typename T>
struct IsLiteralType<T, typename IsLiteralTypeHelper<T>::Type> {
    static constexpr bool value = true;
};

template <typename T>
struct IsConstexprHelper {
    static constexpr T    _test{};
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsLiteralTypeV = IsLiteralType<T>::value;
}  // namespace My

#define CONCEPT_CHECK
#endif