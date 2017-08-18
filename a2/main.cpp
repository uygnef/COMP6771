#include <iostream>
#include "EuclideanVector.h"
#include <list>
int main() {
    std::cout << "Hello, World!" << std::endl;
    std::list<double> v2 {4,5,6,7};
    EuclideanVector a(v2.begin(), v2.end());
    EuclideanVector b = std::move(a);
    std::cout<<*(++b.magnitude.begin())<<"  " <<b.getEuclideanNorm()<<std::endl;

    EuclideanVector c = b.createUnitVector();
    for(auto i:c.magnitude){
        std::cout << i;
    }
    c[1] = 2;
    std::cout << "lala"<< c[1] <<std::endl;
    c *= 3;
    std::cout << c[1] <<std::endl;

    c /= 2;
    std::cout << c[1] <<std::endl;
    return 0;
}