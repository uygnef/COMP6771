#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
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
	for(auto i: values){
		std::cout << i << ", ";
	}	
    }
    assert(myvec.getNumDimensions() == values.size());
    for (auto i = 0U; i < values.size(); i++) {
	if(!almost_eq(myvec.get(i), values.at(i))){
		std::cout << "not equal" << myvec << " " << values.at(i) << std::endl; 
	}
        assert(almost_eq(myvec.get(i), values.at(i)));
    }
}

#endif
