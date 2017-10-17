//
// Created by yu on 16/10/17.
//

#ifndef A5_BUCKETSORT_H
#define A5_BUCKETSORT_H
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>
#include <iostream>

class BucketSort {
    // vector of numbers
public:
    std::vector<unsigned int> numbersToSort;

    void sort(int CoreNum);

private:
    void thread_sort(size_t offset, size_t vector_len);

    inline std::pair<unsigned int, int> getMax();

    inline int get_total_digits(unsigned int);

    inline int get_digits_in_pos(unsigned int num, int pos, const int&);

    inline void merge_all(int total_thread, unsigned long step);

};


#endif //A5_BUCKETSORT_H
