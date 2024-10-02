#pragma once
#ifndef REFLECT_DATA_NODE
#include "reflect_data_traits.h"
#include <array>
#include <cstddef>
#include <string_view>
#include <utility>

namespace My {
template <size_t Idx, typename ValueType>
struct ReflectDataNode {
public:
    ValueType value_;

    constexpr ReflectDataNode() : value_() {}

    constexpr ReflectDataNode( ValueType&& value ) : value_( std::forward<ValueType>( value ) ) {}

    constexpr ReflectDataNode( const ReflectDataNode& value ) : value_( value.value_ ) {}

    constexpr ReflectDataNode( ReflectDataNode&& value ) : value_( std::move( value.value_ ) ) {}

    constexpr ReflectDataNode& operator=( ValueType&& value ) {
        *this = std::forward<ValueType>( value );
        return *this;
    }

    constexpr ReflectDataNode& operator=( const ReflectDataNode& value ) {
        if ( &value != this ) {
            *this->value_ = value.value_;
        }
        return *this;
    }

    constexpr ReflectDataNode& operator=( ReflectDataNode&& value ) {
        if ( &value != this ) {
            *this->value_ = std::move( value.value_ );
        }
        return *this;
    }

    void SetValue( ValueType&& value ) {
        this->value_ = std::forward<ValueType>( value );
    }
};

template <typename KeyType,
          size_t                                         Len,
          size_t                                         Idx,
          const std::array<GetLiteralType<KeyType>, Len> keyArr,
          typename Head,
          typename... Tails>
struct ReflectDataImple {
public:
    using NextType = ReflectDataImple<KeyType, Len, Idx + 1, keyArr, Tails...>;
    using NodeType = ReflectDataNode<Idx, Head>;
    NodeType node_;
    NextType next_;

    constexpr ReflectDataImple() : node_(), next_() {}

    constexpr ReflectDataImple( NodeType&& node, NextType&& next )
    : node_( std::forward<NodeType>( node ) ), next_( std::forward<NextType>( next ) ) {}
};

/* template <typename KeyType, size_t Len, const std::array<const std::string_view, Len> keyArr, typename... TArgs>
struct ReflectDataImple {};

constexpr std::string_view                       view1 = "123";
constexpr std::array<const std::string_view*, 1> kTest{ &view1 };
constexpr std::array<const int, 1>               kTest2{ 1 };
ReflectDataImple<int, 1, kTest, int, float>      hhh; */
}  // namespace My

#define REFLECTDATA_NODE
#endif