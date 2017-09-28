#include <iostream>
#include <set>
#include <memory>
#include <iterator>
#include "btree.h"



int main() {
    std::cout << "Hello, World!" << std::endl;
    btree<long> testContainer(3);
    std::cout << "123";
    for(long i = 0; i < 10; ++i){
        std::cout << i;
        testContainer.insert(i);
    }


//    std::set<std::shared_ptr<node>, compare> a;
//    auto node1 = std::make_shared<node>(node{1});
//    auto node2 = std::make_shared<node>(node{2});
//    a.insert(node1);
//    a.insert(node2);
//    auto i = a.find(node1);
//    i->get()->val = 0;
//    for(const auto& i: a){
//        std::cout << i.get()->val;
//    }
    std::cout << "1\n";

    return 0;
}