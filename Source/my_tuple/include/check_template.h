#ifndef CHECK_TEMPLATE
#include "type_traits"
#include <string_view>
#include <vector>

// 判断一个模板是否是另外一个模板
template <typename, template <typename...> typename>
struct IsTemplateHelper {
    static constexpr bool value = false;
};

template <template <typename...> typename Target, typename... TArgs>
struct IsTemplateHelper<Target<TArgs...>, Target> {
    static constexpr bool value = true;
};

template <typename T, template <typename...> typename U>
constexpr bool IsTemlateV = IsTemplateHelper<T, U>::value;

template <typename T>
constexpr bool IsVector = IsTemplateHelper<T, std::vector>::value;

// 判断是否时Array对象的SFINAE helper，非偏特化的false分支
template <typename, template <typename, std::size_t> typename>
struct IsArrayHelper {
    static constexpr bool value = false;
};

// 这里会通过TArray<T,kSize>来推断kSize
template <template <typename, std::size_t> typename TArray, typename T, std::size_t kSize>
struct IsArrayHelper<TArray<T, kSize>, TArray> {
    static constexpr bool value = true;
};

template <typename T, template <typename, std::size_t> typename TArray>
constexpr bool IsArrayHelperV = IsArrayHelper<T, TArray>::value;

template <typename T>
constexpr bool IsArray = IsArrayHelperV<T, std::array>;
template <std::size_t kSize>
constexpr std::array<std::string_view, kSize> tArray;

// 非特化的array判断声明完毕后使用
template <template <typename, std::size_t> typename TArray, std::size_t kSize>
struct IsStringViewArrayHelper : public IsArrayHelper<TArray<std::string_view, kSize>, TArray> {};

// 判断是否时反射对象的SFINAE helper，非偏特化的false分支
template <typename, template <size_t kSize, const std::array<std::string_view, kSize>*, typename...> typename>
struct IsReflectDataHelper {
    static constexpr bool value = false;
};

// 判断是否时反射对象的SFINAE helper，偏特化的true分支
template <template <size_t kSize, const std::array<std::string_view, kSize>*, typename...> typename TReflect,
          std::size_t                                kSize,
          const std::array<std::string_view, kSize>* kArray,
          typename... TArgs>
struct IsReflectDataHelper<TReflect<kSize, kArray, TArgs...>, TReflect> {
    static constexpr bool value = true;
};

// 判断是否时反射对象的编译期模板变量helper
template <typename T,
          template <size_t kSize, const std::array<std::string_view, kSize>*, typename...>
          typename TReflect>
constexpr bool IsReflectHelperV = IsReflectDataHelper<T, TReflect>::value;
#define CHECK_TEMPLATE
#endif
