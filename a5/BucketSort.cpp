//
// Created by yu on 16/10/17.
//

#include "BucketSort.h"

#include <algorithm>
#include <cmath>

bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {

    if (x == y) return false; // if the two numbers are the same then one is not less than the other

    unsigned int a = x;
    unsigned int b = y;

    // work out the digit we are currently comparing on.
    if (pow == 0) {
        while (a / 10 > 0) {
            a = a / 10;
        }
        while (b / 10 > 0) {
            b = b / 10;
        }
    } else {
        while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            a = a / 10;
        }
        while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            b = b / 10;
        }
    }

    if (a == b)
        return aLessB(x,y,pow + 1);  // recurse if this digit is the same
    else
        return a < b;
}

// TODO: replace this with a parallel version.
void BucketSort::sort(unsigned int numCores) {
    // split in numCores buckets.

    // sort in spearate buckets.

    // join
    std::sort(numbersToSort.begin(),numbersToSort.end(), [](const unsigned int& x, const unsigned int& y){
        return aLessB(x,y,0);
    } );
}