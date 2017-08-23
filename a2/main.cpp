#include <iostream>
#include <vector>
#include <list>

#include "EuclideanVector.h"

constexpr double eps = 1e-7;

inline bool almost_eq(double a, double b) {
    return std::abs(a - b) < eps;
}

inline void assertEquals(const evec::EuclideanVector& myvec, std::vector<double> values) {
    int vec_size = myvec.getNumDimensions();
    int values_size = values.size();
    if(vec_size != values_size) {
        std::cout << "Vector length " << vec_size << "\n";
        std::cout << "Values length " << values_size << "\n";
    }
    assert(myvec.getNumDimensions() == values.size());
    for (auto i = 0U; i < values.size(); i++) {
        if(!almost_eq(myvec.get(i), values.at(i))){
            std::cout << "not equal" << myvec.get(i) << values.at(i) << std::endl;
        }
        assert(almost_eq(myvec.get(i), values.at(i)));
    }
}


int main() {


    {
        // Scalar multiply an rvalue.
   //     evec::EuclideanVector a = evec::EuclideanVector(2, 2.0) * 4.0;
        assertEquals(evec::EuclideanVector(2, 2.0) * 2.0, {4.0, 4.0});
    }

}