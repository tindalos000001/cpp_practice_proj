#pragma once
#include <tuple>
#ifndef MAYBE_MONAD
#define MAYBE_MONAD
#include <fstream>
#include <type_traits>
#include <variant>

namespace My::Monad {
// 定义构造Visitor时所需的捕获lambda入参类型的辅助类型
template <typename... TS>
struct OverLoad : TS... {
    using TS::operator()...;
};
// 定义Maybe类型和所需的Just以及Nothing类型
template <typename T>
struct Just {
    T value;
    operator const T&() const {
        return value;
    }
    Just( const T& param ) : value( param ) {}
};
struct Nothing {};
struct AnyType {
    template <typename T>
    operator T();

    friend std::ostream& operator<<( std::ostream& os, const AnyType& ) {
        return os;
    };
};
template <typename T>
struct Maybe {
    std::variant<Just<T>, Nothing> value;
    Maybe( const T& param ) : value( param ) {}
    Maybe( const Nothing& ) : value( Nothing() ) {}
    // 重载Maybe的ofstream流输出函数用于打印
    friend std::ostream& operator<<( std::ostream& os, const Maybe<T>& param ) {
        auto res = std::visit(
        OverLoad{ [ & ]( const Just<T>& param ) -> T { return param.value; }, []( Nothing ) -> T { return T(); } },
        param.value );
        os << res;
        return os;
    };
};
template <typename T>
auto MakeJust( const T& value ) -> Maybe<T> {
    return value;
}
inline auto MakeNothing() -> Maybe<AnyType> {
    return Nothing{};
}
}  // namespace My::Monad
#endif