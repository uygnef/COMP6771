#include <iostream>
#include "btree.h"
#include <random>


int main() {
    std::cout << "Hello, World!" << std::endl;
    btree<int> a(2);
    srand(100);
    for(int i=0; i < 16; ++i){
        auto sa = rand() % 100;
        std::cout << sa << " ";
        a.insert(sa);
    }
    std::cout << std::endl;


    std::cout << a;
    std::cout << std::endl;

    btree<int> c{a};
    btree<int> d{std::move(c)};

    auto e = d;
    auto f = std::move(e);

    return 0;
}