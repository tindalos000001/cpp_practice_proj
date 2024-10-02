#pragma once
#ifndef REFLECT_DATA_TRAITS
#include "common/concept_check.h"

namespace My {

namespace LITERALHELPERVISIBILITY {
    template <typename T, bool Value>
    struct ToLiteralType {};

    template <typename T>
    struct ToLiteralType<T, true> {
        using Type = T;
    };

    template <typename T>
    struct ToLiteralType<T, false> {
        using Type = T*;
    };
}  // namespace LITERALHELPERVISIBILITY

template <typename T>
using GetLiteralType = LITERALHELPERVISIBILITY::ToLiteralType<T, IsLiteralTypeV<T>>::Type;

}  // namespace My

#define REFLECT_DATA_TRAITS
#endif