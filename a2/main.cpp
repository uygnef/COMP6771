#include <iostream>
#include "EuclideanVector.h"
#include <list>
int main() {
    std::cout << "Hello, World!" << std::endl;
    EuclideanVector a{3,4.0};
    std::cout<<a.magnitude[2]<<std::endl;

    std::list<double> b{11,12,13,14};
    EuclideanVector c{b.begin(), b.end()};
    std::cout<<c.magnitude[2]<<std::endl;

    EuclideanVector d{c};
    std::cout<<d.magnitude[1]<<std::endl;

    EuclideanVector e = std::move(c);
    std::cout<<c.magnitude[2]<<std::endl;
    std::cout<<e.magnitude[2]<<std::endl;


    return 0;
}