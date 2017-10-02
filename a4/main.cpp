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
        std::cout << "\n----"<< i << std::endl;
        testContainer.insert(i);
    }

    long s = 3;
    auto a = testContainer.find(s);
    std::cout << "\n find s:"<< a.it->get()->val;
    auto c(testContainer);
    a = c.find(s);
    std::cout << "\n ---still finds :-"<< a.it->get()->val;

    for(long i = 11; i < 20; ++i){
        std::cout << i;
        c.insert(i);
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

    auto d(std::move(c));
    std::cout << "move success\n";
    a = c.find(s);
    std::cout << "move -sd\n";
    if(a == c.end()){
        std::cout <<"not have\n";
    }

    for(auto i=d.begin(); i != d.end(); ++i){
        if(i.it->get()->val == 18)
            std::cout <<"\n go through:"<< i.it->get()->val <<std::endl;
    }
    a = d.find(11);
    std::cout << "\n ----"<< a.it->get()->val;

    d = testContainer;
    a = d.find(9);
    std::cout << "\n --2sa--"<< a.it->get()->val<<"\n";

    d = std::move(testContainer);

    auto sfd = d.begin();
    std::cout << sfd.it->get()->val;
    ++sfd;
    std::cout << sfd.it->get()->val;

    std::cout << "finish\n";
    return 0;
}