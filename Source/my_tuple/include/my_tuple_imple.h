#ifndef MY_TUPLE_IMPLE
#include <crtdefs.h>
#include <cstddef>
#include <type_traits>
#include <utility>

// 获取变长类型参数的指定位置类型
template <size_t idx, typename Head, typename... Tails>
struct CatchType : public CatchType<idx - 1, Tails...> {
public:
};

template <typename Head, typename... Tails>
struct CatchType<0, Head, Tails...> {
public:
    using ValueType = Head;
};

template <size_t idx, typename... TArgs>
using IndexType = typename CatchType<idx, TArgs...>::ValueType;

template <typename _Tp>
inline constexpr bool IsEmpty = std::is_empty_v<_Tp>;
template <typename _Tp>
inline constexpr bool IsNotFinal = !std::is_final_v<_Tp>;
template <typename _Tp>
inline constexpr bool IsEmptyButNotFinal = IsEmpty<_Tp> && IsNotFinal<_Tp>;

/// @brief
/// 存储节点基类，用于存储CRTP中捕获的类型参数对应的数据,为了空基类优化使用了偏特化
/// @tparam _Head
/// @tparam idx
template <size_t idx,
          typename _Head,
          bool IsTypeVoid   = IsEmptyButNotFinal<_Head>,
          bool IsRefernence = std::is_reference<_Head>::value>
struct HeadBase {};

/// @brief 存储类型为空且不为final的偏特化情况
/// @tparam _Head
/// @tparam idx
template <size_t idx, typename _Head>
struct HeadBase<idx, _Head, true, false> : public _Head {
    using ValueType = _Head;

    constexpr HeadBase() : _Head() {}

    // 容器类型左值构造
    constexpr HeadBase( const _Head& h ) : _Head( h ) {}

    // 容器类型右值构造
    template <typename UHead>
    constexpr HeadBase( UHead&& h ) : _Head( std::forward<UHead>( h ) ) {}

    // 赋值构造
    constexpr HeadBase& operator=( ValueType& v ) {
        if ( &v != this ) {
            ValueType* base = static_cast<ValueType*>( this );
            *base           = v;
        }
        return *this;
    }

    constexpr HeadBase& operator=( ValueType&& v ) {
        if ( &v != this ) {
            ValueType* base = static_cast<ValueType*>( this );
            *base           = std::move( v );
        }
        return *this;
    }

    void SetValue( ValueType& v ) {
        *this = v;
    }

    void SetValue( ValueType&& v ) {
        *this = std::move( v );
    }

    constexpr HeadBase( const HeadBase& hb ) = default;
    constexpr HeadBase( HeadBase&& hb )      = default;

    static constexpr _Head& Value( HeadBase& hb ) noexcept {
        return hb;
    }

    static constexpr const _Head& Value( const HeadBase& hb ) noexcept {
        return hb;
    }
};

/// @brief 存储类型不为空或者为空但为final的偏特化情况
/// @tparam _Head
/// @tparam idx
template <size_t idx, typename _Head>
struct HeadBase<idx, _Head, false, false> {
    using ValueType = _Head;

    constexpr HeadBase() : value() {}

    // 容器类型左值构造
    constexpr HeadBase( const _Head& h ) : value( h ) {}

    // 容器类型右值构造
    template <typename UHead>
    constexpr HeadBase( UHead&& h ) : value( std::forward<UHead>( h ) ) {}

    // 赋值构造
    constexpr HeadBase& operator=( ValueType& v ) {
        if ( &v != this ) {
            value = v;
        }
        return *this;
    }

    constexpr HeadBase& operator=( ValueType&& v ) {
        if ( &v != this ) {
            value = std::move( v );
        }
        return *this;
    }

    void SetValue( ValueType& v ) {
        value = v;
    }

    void SetValue( ValueType&& v ) {
        value = std::move( v );
    }

    constexpr HeadBase( const HeadBase& hb ) = default;
    constexpr HeadBase( HeadBase&& hb )      = default;

    static constexpr _Head& Value( HeadBase& hb ) noexcept {
        return hb.value;
    }

    static constexpr const _Head& Value( const HeadBase& hb ) noexcept {
        return hb.value;
    }

    _Head value;
};

/// @brief
/// 存储类型为空且不为final的偏特化情况,针对引用类型，针对引用类型，右值构造会造成引用折叠引起重复定义的情况，因此需要去除
/// @tparam _Head
/// @tparam idx
template <size_t idx, typename _Head>
struct HeadBase<idx, _Head, true, true> : public _Head {
    using ValueType = _Head;

    constexpr HeadBase( _Head h ) : _Head( h ) {}

    // 赋值构造
    constexpr HeadBase& operator=( ValueType& v ) {
        if ( &v != this ) {
            ValueType* base = static_cast<ValueType*>( this );
            *base           = v;
        }
        return *this;
    }

    void SetValue( ValueType& v ) {
        *this = v;
    }

    constexpr HeadBase( const HeadBase& hb ) = default;
    constexpr HeadBase( HeadBase&& hb )      = default;

    static constexpr _Head& Value( HeadBase& hb ) noexcept {
        return hb;
    }

    static constexpr const _Head& Value( const HeadBase& hb ) noexcept {
        return hb;
    }
};

/// @brief
/// 存储类型不为空或者为空但为final的偏特化情况,针对引用类型，右值构造会造成引用折叠引起重复定义的情况，因此需要去除
/// @tparam _Head
/// @tparam idx
template <size_t idx, typename _Head>
struct HeadBase<idx, _Head, false, true> {
    using ValueType = _Head;

    constexpr HeadBase( _Head& h ) : value( h ) {}

    // 赋值构造
    constexpr HeadBase& operator=( ValueType& v ) {
        value = v;
        return *this;
    }

    void SetValue( ValueType& v ) {
        value = v;
    }

    constexpr HeadBase( const HeadBase& hb ) = default;
    constexpr HeadBase( HeadBase&& hb )      = default;

    static constexpr _Head& Value( HeadBase& hb ) noexcept {
        return hb.value;
    }

    static constexpr const _Head& Value( const HeadBase& hb ) noexcept {
        return hb.value;
    }

    _Head value;
};

/// @brief TupleImple对变长的模板类型参数进行捕获索引用来标识这是第几个类型参数
/// @tparam _Tails
/// @tparam _Head
/// @tparam idx
template <size_t idx, typename _Head, typename... _Tails>
struct MyTupleImple : public MyTupleImple<idx + 1, _Tails...>, HeadBase<idx, _Head> {
public:
    using InheritedType = MyTupleImple<idx + 1, _Tails...>;
    using BaseType      = HeadBase<idx, _Head>;

    // 需要声明四种构造方法
    // 1. 使用父类和节点基类构造
    // 2. 逐节点copy构造
    // 3. 逐节点move构造
    // 4. 拷贝构造

    constexpr _Head& GetValue() {
        return this->BaseType::Value( static_cast<BaseType&>( *this ) );
    }

    // 空构造函数
    constexpr MyTupleImple() : InheritedType(), BaseType() {}

    // 容器类型住节点复制构造
    constexpr MyTupleImple( const _Head& head, const _Tails&... tails ) : InheritedType( tails... ), BaseType( head ) {}

    // 容器类型逐节点移动构造
    // eanble_if既可以使用enable_if_t<...,T> =
    // T类型的值，此时错误匹配就会导致默认值类型错误，
    // 正确匹配则T类型默认值为T类型的值通过
    // 也可以使用typename = typename std::enable_if_t<...>
    // 此时匹配错误时enable_if_t不存在，匹配失败
    // 匹配成功时,enable_if_t=enable_if<true,void>::type即void，匹配通过
    template <typename _UHead,
              typename... _UTails,
              typename = typename std::enable_if_t<sizeof...( _Tails ) == sizeof...( _UTails )>>
    explicit constexpr MyTupleImple( _UHead&& head, _UTails&&... tails )
    : InheritedType( std::forward<_UTails>( tails )... ), BaseType( std::forward<BaseType>( head ) ) {}

    // 拷贝构造
    constexpr MyTupleImple( const MyTupleImple<idx, _Head, _Tails...>& src )
    : BaseType( dynamic_cast<const BaseType&>( src ) ), InheritedType( dynamic_cast<const InheritedType&>( src ) ) {}

    // 移动构造
    constexpr MyTupleImple( MyTupleImple&& tuple )
    : BaseType( static_cast<BaseType&&>( tuple ) ),
      InheritedType( std::move( static_cast<InheritedType&&>( tuple ) ) ) {}
};

/// @brief 最后一个类型参数时的偏特化
/// @tparam _Head
/// @tparam idx
template <size_t idx, typename _Head>
struct MyTupleImple<idx, _Head> : public HeadBase<idx, _Head> {
public:
    using BaseType = HeadBase<idx, _Head>;

    // 空构造函数
    constexpr MyTupleImple() : BaseType() {}

    // 节点类型完美转发构造
    constexpr MyTupleImple( const BaseType& base ) : BaseType( std::forward<BaseType>( base ) ) {}

    // 容器类型左值构造
    constexpr MyTupleImple( const _Head& head ) : BaseType( head ) {}

    // 容器类型右值构造
    template <typename _UHead>
    constexpr MyTupleImple( _UHead&& head ) : BaseType( std::forward<_UHead>( head ) ) {}

    // 拷贝构造
    constexpr MyTupleImple( const MyTupleImple& tuple ) : BaseType( dynamic_cast<const BaseType&>( tuple ) ) {}

    // 移动构造
    constexpr MyTupleImple( MyTupleImple&& tuple ) : BaseType( dynamic_cast<BaseType&&>( tuple ) ) {}

    _Head& GetValue() {
        return this->BaseType::Value( static_cast<BaseType&>( *this ) );
    }
};

#define MY_TUPLE_IMPLE
#endif
