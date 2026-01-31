#include "Source/common/include/common/concept_check.h"
#include "common/check_template.h"
#include "common/concept_check.h"
#include "my_tuple/my_tuple.h"
#include <iostream>
#include <maybe_monad/maybe_monad.h>
#include <memory>
#include <utility>
#include <variant>
#include <vector>


struct EmptyBase {};

template <typename T>
void Print( const T& arg, size_t i ) {
    std::cout << i << ": " << arg << "\n";
}

template <typename... Args, size_t... I>
void PrintAll( std::index_sequence<I...>, Args... args ) {
    ( Print( args, I ), ... );
    std::cout << "\n";
}

template <typename... Args>
void PrintAllByIndex( Args... args ) {
    const size_t index = sizeof...( args );
    PrintAll( std::make_index_sequence<index>(), args... );
}
// 使用std::visit的编译期参数重载，如果传入的Maybe类型是Just状态则执行函数1，如果传入的Maybe类型是Nothing类型则执行函数2
template <typename T, typename F>
auto operator >> ( const My::Monad::Maybe<T>& m, F&& func ) {
    using ResultT = std::invoke_result_t<F, T>;
    return std::visit( My::Monad::OverLoad{ [ & ]( const My::Monad::Just<T>& v ) -> ResultT { return func( static_cast<T>( v ) ); },
                                 []( My::Monad::Nothing ) -> ResultT { return My::Monad::Nothing{}; } },
                       m.value );
}
int main() {
    // std::cout << "hello world";
    // std::cout << "build successfully" << "\n";
    // double                                    d   = 10.0;
    // int                                       i   = 1;
    // int                                       i2  = 10;
    // std::string                               str = "hahaha";
    // My::MyTuple<double, int, std::string>     test( d, i, str );
    // My::MyTuple<double, int, std::string>     test2 = test;
    // My::MyTuple<double, int, std::string>     test3 = std::move( test );
    // My::MyTuple<double, std::unique_ptr<int>> test4{ 10, nullptr };
    // auto&                                     testV = test3.Get<2>();
    // test3.Set<2>( "123123123" );
    // std::unique_ptr<int> iPtr = std::unique_ptr<int>( new int( 10 ) );
    // test4.Set<1>( std::move( iPtr ) );
    // My::MyTuple<EmptyBase, int> k;
    // EmptyBase                   e;
    // k.Set<0>( e );
    // k.Set<1>( 1 );
    // std::cout << "testV:" << test3.Get<2>() << ",testPtrV:" << *( test4.Get<1>().get() ) << "k int v:" << k.Get<1>();
    // bool tK = IsVector<std::vector<int>>;
    // std::cout << "std::vector<int> is vector?:" << "\n";
    // PrintAllByIndex( "123", 1, 2.0f, "213" );
    auto param = My::Monad::MakeJust( 2 );
    auto res   = param >>  []( int value ) -> My::Monad::Maybe<double> { return static_cast<double>( value ) * 2.0; }
    >> [](double value)->My::Monad::Maybe<double>{return value * 4.0;};
    std::cout << res;
}