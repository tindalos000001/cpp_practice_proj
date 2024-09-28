#include "check_template.h"
#include "my_tuple.h"
#include "reflect_data.h"
#include <iostream>
#include <memory>
#include <vector>

struct EmptyBase {};

int main() {
    std::cout << "hello world";
    std::cout << "build successfully" << "\n";
    double                                d   = 10.0;
    int                                   i   = 1;
    int                                   i2  = 10;
    std::string                           str = "hahaha";
    MyTuple<double, int, std::string>     test( d, i, str );
    MyTuple<double, int, std::string>     test2 = test;
    MyTuple<double, int, std::string>     test3 = std::move( test );
    MyTuple<double, std::unique_ptr<int>> test4{ 10, nullptr };
    auto&                                 testV = test3.Get<2>();
    test3.Set<2>( "123123123" );
    std::unique_ptr<int> iPtr = std::unique_ptr<int>( new int( 10 ) );
    test4.Set<1>( std::move( iPtr ) );
    MyTuple<EmptyBase, int> k;
    EmptyBase               e;
    k.Set<0>( e );
    k.Set<1>( 1 );
    std::cout << "testV:" << test3.Get<2>() << ",testPtrV:" << *( test4.Get<1>().get() ) << "k int v:" << k.Get<1>();
    bool tK = IsVector<std::vector<int>>;
    std::cout << "std::vector<int> is vector?:" << tK;
}