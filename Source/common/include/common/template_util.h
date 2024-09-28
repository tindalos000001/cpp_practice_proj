#ifndef TEMPLATE_UTIL
#include <cstddef>

namespace My {
// 捕获类型参数包中指定索引类型的工具类
template <size_t kIndex, typename Head, typename... TArgs>
struct CatchType : public CatchType<kIndex - 1, TArgs...> {};

template <typename Head, typename... TArgs>
struct CatchType<0, Head, TArgs...> {
    using Type = Head;
};

template <size_t idx, typename... TArgs>
using IndexType = typename CatchType<idx, TArgs...>::Type;

// 捕获单一类型参数包中指定索引变量的工具类
template <size_t kIndex, typename T, T Head, T ...Args>
struct CatchPackValue : public CatchPackValue<kIndex - 1, T,Args...> {
public:
    using Inherited = CatchPackValue<kIndex - 1, T,Args...>;
};

template <typename T, T Head, T ...Args>
struct CatchPackValue<0, T, Head, Args...> {
    static constexpr T Value = Head;
};

template <size_t idx, typename T ,T ...Args>
constexpr T IndexValue = CatchPackValue<idx, T, Args...>::Value;
constexpr int t = IndexValue<1,int, 1,2,3,4>;
}  // namespace My

#define TEMPLATE_UTIL
#endif