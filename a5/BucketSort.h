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
    // void split(std::size_t start, std::size_t end, std::vector<std::vector<unsigned int>>& , unsigned int numCores);

    std::pair<unsigned int, int> getMax();

    int get_digits(unsigned int);

    inline int get_digits_in_pos(unsigned int num, int pos, const int&);

};


#endif //A5_BUCKETSORT_H
