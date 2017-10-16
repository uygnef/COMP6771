//
// Created by yu on 16/10/17.
//

#ifndef A5_BUCKETSORT_H
#define A5_BUCKETSORT_H
#include <vector>


class BucketSort {
    // vector of numbers
public:
    std::vector<unsigned int> numbersToSort;

    void sort(unsigned int numCores);

private:
    void split(unsigned int numCores);
};


#endif //A5_BUCKETSORT_H
