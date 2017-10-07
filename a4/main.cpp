#include <iostream>
#include "btree.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    btree<int> a(2);
    for(int i=0; i < 100; ++i){
        std::cout << i << std::endl;
        a.insert(i);
    }
    btree<int> c{a};
    return 0;
}