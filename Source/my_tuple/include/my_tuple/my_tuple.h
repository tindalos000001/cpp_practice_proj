#ifndef MY_TUPLE
#include "my_tuple_imple.h"
#include "common/template_util.h"
namespace My {
template <typename... TArgs>
struct MyTuple : public MyTupleImple<0, TArgs...> {
public:
    using ImpleType = MyTupleImple<0, TArgs...>;

    template <size_t idx, std::enable_if_t<( idx < sizeof...( TArgs ) ), bool> = true>
    constexpr IndexType<idx, TArgs...>& Get() {
        return this->MyTupleImple<idx, IndexType<idx, TArgs...>>::GetValue();
    }

    template <size_t idx, std::enable_if_t<( idx < sizeof...( TArgs ) ), bool> = true>
    constexpr void Set( IndexType<idx, TArgs...>& param ) {
        this->HeadBase<idx, IndexType<idx, TArgs...>>::SetValue( param );
    }

    template <size_t idx, typename _UType, std::enable_if_t<( idx < sizeof...( TArgs ) ), bool> = true>
    constexpr void Set( _UType&& param ) {
        this->HeadBase<idx, IndexType<idx, TArgs...>>::SetValue( std::forward<_UType>( param ) );
    }

    // 变参左值构造
    constexpr MyTuple( TArgs&... tArgs ) : ImpleType( std::template forward<TArgs>( tArgs )... ) {}

    // 变参右值构造
    template <typename... UTArgs>
    constexpr MyTuple( UTArgs&&... args ) : ImpleType( std::forward<UTArgs>( args )... ) {}

    constexpr MyTuple() : ImpleType() {}

    constexpr MyTuple( const MyTuple& tuple ) : ImpleType( dynamic_cast<const ImpleType&>( tuple ) ) {}

    constexpr MyTuple( MyTuple&& tuple ) : ImpleType( dynamic_cast<ImpleType&&>( tuple ) ) {}
};
}

#define  MY_TUPLE
#endif